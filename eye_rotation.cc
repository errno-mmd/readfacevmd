#include <LandmarkCoreIncludes.h>
#include <FaceAnalyser.h>
#include <SequenceCapture.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace std;

bool find_single_pupil(cv::Mat& image_eye, cv::Point2d& pupil_point)
{
  cv::Mat image_gray;
  cv::cvtColor(image_eye, image_gray, cv::COLOR_BGR2GRAY);
  cv::equalizeHist(image_gray, image_gray);

  cv::medianBlur(image_gray, image_gray, 3);
  cv::Mat element_close = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::morphologyEx(image_gray, image_gray, cv::MORPH_CLOSE, element_close);
  cv::Mat element_open = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::morphologyEx(image_gray, image_gray, cv::MORPH_OPEN, element_open);
  cv::Mat image_binary;
  const int pupil_threshold = 80;
  cv::threshold(image_gray, image_binary, pupil_threshold, 255, cv::THRESH_BINARY_INV);

  vector<vector<cv::Point>> contours;
  cv::findContours(image_binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

  //cv::drawContours(image_eye, contours, -1, cv::Scalar(255, 128, 128), 1);
  int idx_max = -1;
  float max_area = 0;
  for (int i = 0; i < (int)contours.size(); i++) {
    float area = cv::contourArea(contours[i]);
    if (area > max_area) {
      idx_max = i;
      max_area = area;
    }
  }
  if (idx_max == -1) {
    return false;
  }

  //cv::drawContours(image_eye, contours, idx_max, cv::Scalar(128, 255, 128), 3);

  cv::Moments moments = cv::moments(contours[idx_max]);
  if (moments.m00 == 0.0) {
    cerr << "moments.m00 is zero" << endl;
    return false;       
  }
  double x = moments.m10 / moments.m00;
  double y = moments.m01 / moments.m00;
  pupil_point = cv::Point2d(x, y);
  //cv::circle(image_eye, pupil_point, 10, cv::Scalar(0, 0, 255), 3);
  return true;
}

void make_eye_image(const cv::Mat& image_src, cv::Mat& image_eye, cv::Rect& rect, const cv::Mat_<float>& detected_landmarks, bool left)
{
  int landmark_start;
  if (left) {
    landmark_start = 42;
  } else {
    landmark_start = 36;
  }
  const int eye_landmark_num = 6;
  int landmark_num = detected_landmarks.total() / 2;
  vector<vector<cv::Point>> landmark(1);
  for (int i = 0; i < eye_landmark_num; i++) {
    cv::Point p((int)detected_landmarks[0][landmark_start + i], (int)detected_landmarks[0][landmark_num + landmark_start + i]);
    landmark[0].push_back(p);
  }
  rect = cv::boundingRect(landmark[0]);

  vector<vector<cv::Point>> landmark_in_rect(1);
  for (int i = 0; i < eye_landmark_num; i++) {
    cv::Point p(landmark[0][i].x - rect.x, landmark[0][i].y - rect.y);
    landmark_in_rect[0].push_back(p);
  }

  image_eye = image_src(rect).clone();

  cv::Mat image_mask(image_eye.size(), image_eye.type(), cv::Scalar::all(0));
  cv::fillPoly(image_mask, landmark_in_rect, cv::Scalar::all(255));
  cv::Mat inverted_mask;
  cv::bitwise_not(image_mask, inverted_mask);
  cv::bitwise_or(image_eye, inverted_mask, image_eye);
}

void calc_eye_rotation(Eigen::Quaterniond& rot, const cv::Point2d& point_pupil, int width, int height, const Eigen::Quaterniond& head_rot)
{
  const double eye_width_to_radius = 0.7;
  double r = width * eye_width_to_radius;
  if (height > width) {
    r = height * eye_width_to_radius;
  }
  double x = point_pupil.x - (width / 2);
  double y = point_pupil.y - (height / 2);

  double a1 = asin(x / r);
  double z = r * cos(a1);
  double a2 = asin(y / z);

  const double adjuster_coeff = 0.5;
  Eigen::Quaterniond rot_adjuster = Eigen::Quaterniond::Identity().slerp(adjuster_coeff, head_rot);
  rot = Eigen::AngleAxisd(a2, -Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(a1, -Eigen::Vector3d::UnitY()) * rot_adjuster.inverse();
}

bool eye_is_open(const cv::Mat_<float>& landmarks, bool left)
{
  double width;
  double height;
  int landmark_num = landmarks.total() / 2;
  if (left) {
    cv::Point2d p1(landmarks[0][45], landmarks[0][landmark_num + 45]);
    cv::Point2d p2(landmarks[0][42], landmarks[0][landmark_num + 42]);
    width = cv::norm(p1 - p2);
    cv::Point2d p3(landmarks[0][43], landmarks[0][landmark_num + 43]);
    cv::Point2d p4(landmarks[0][47], landmarks[0][landmark_num + 47]);
    height = cv::norm(p3 - p4);
  } else {
    cv::Point2d p1(landmarks[0][39], landmarks[0][landmark_num + 39]);
    cv::Point2d p2(landmarks[0][36], landmarks[0][landmark_num + 36]);
    width = cv::norm(p1 - p2);
    cv::Point2d p3(landmarks[0][38], landmarks[0][landmark_num + 38]);
    cv::Point2d p4(landmarks[0][40], landmarks[0][landmark_num + 40]);
    height = cv::norm(p3 - p4);
  }

  const double threshold = 0.2;
  if (height > width * threshold) {
    return true;
  }
  return false;
}

bool estimate_eye_pose(Eigen::Quaterniond& rot, const cv::Mat& image, const Eigen::Quaterniond& head_rot, const cv::Mat_<float>& detected_landmarks, bool left)
{
  cv::Rect rect;
  cv::Mat image_eye;
  cv::Point2d point_pupil;
  if (!eye_is_open(detected_landmarks, left)) {
      return false;
  }
  make_eye_image(image, image_eye, rect, detected_landmarks, left);
  find_single_pupil(image_eye, point_pupil);
  calc_eye_rotation(rot, point_pupil, rect.width, rect.height, head_rot);
  cv::Point2d left_pupil_center(point_pupil.x + rect.x, point_pupil.y + rect.y);
  return true;
}
