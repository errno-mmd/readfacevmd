#include <GazeEstimation.h>
#include <RotationHelpers.h>
#include <opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <LandmarkDetectorFunc.h>
#include <LandmarkDetectorModel.h>
#include "MyGazeEstimation.h"

using namespace Eigen;

void MyGazeEstimation::EstimateGaze(const LandmarkDetector::CLNF& clnf, cv::Point3f& gaze, float fx, float fy, float cx, float cy, bool is_left)
{
    cv::Vec6f head = LandmarkDetector::GetPose(clnf, fx, fy, cx, cy);
    cv::Matx33f rot = Utilities::Euler2RotationMatrix(cv::Vec3f(head(3), head(4), head(5)));

    int part = -1;
    for (size_t i = 0; i < clnf.hierarchical_models.size(); i++) {
        if (is_left) {
            if (clnf.hierarchical_model_names[i] == "left_eye_28") {
                part = i;
            }
        } else {
            if (clnf.hierarchical_model_names[i] == "right_eye_28") {
                part = i;
            }
        }
    }
    if (part == -1) {
        gaze = cv::Point3f(0, 0, 0);
        return;
    }

    cv::Mat eye_landmarks = clnf.hierarchical_models[part].GetShape(fx, fy, cx, cy);
    cv::Point3f pupil = GazeAnalysis::GetPupilPosition(eye_landmarks);
    cv::Point3f ray_dir = pupil / norm(pupil);
    cv::Mat face_landmarks = clnf.GetShape(fx, fy, cx, cy).t();

    int eye_index = 1;
    if (is_left) {
        eye_index = 0;
    }

    cv::Mat offset_mat = (cv::Mat_<float>(3, 1) << 0.0, -3.5, 7.0);
    cv::Mat offset_mat_t = (cv::Mat(rot) * offset_mat ).t();
    cv::Point3f eye_offset = cv::Point3f(offset_mat_t);
    cv::Mat mat_l = face_landmarks.row(36 + eye_index * 6);
    cv::Mat mat_r = face_landmarks.row(39 + eye_index * 6);
    cv::Point3f lid_l = cv::Point3f(mat_l);
    cv::Point3f lid_r = cv::Point3f(mat_r);
    cv::Point3f eye_center = (lid_l + lid_r) / 2.0;
    cv::Point3f eyeball_center = eye_center + eye_offset;

    // 2Dに再投影
    float d = eye_center.z;
    float l2dx = lid_l.x * d / lid_l.z;
    float l2dy = lid_l.y * d / lid_l.z;
    float r2dx = lid_r.x * d / lid_r.z;
    float r2dy = lid_r.y * d / lid_r.z;
    float p2dx = pupil.x * d / pupil.z;
    float p2dy = pupil.y * d / pupil.z;
    float t = (p2dx - r2dx) / (l2dx - r2dx);
    if (t < 0.0) t = 0.0; else if (t > 1.0) t = 1.0;
    float newZ = lid_r.z + (lid_l.z - lid_r.z) * t;
    // 新しいzで、黒目の中心位置を再計算する。
    pupil.x = pupil.x * newZ / pupil.z;
    pupil.y = pupil.y * newZ / pupil.z;
    pupil.z = newZ;
    ray_dir = pupil / norm(pupil); 

    cv::Point3f gaze_axis = pupil - eyeball_center;

    gaze = gaze_axis / norm(gaze_axis);
}

