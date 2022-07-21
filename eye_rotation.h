// -*- C++ -*-
// 視線推定

#ifndef EYE_ROTATION_H
#define EYE_ROTATION_H

#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

bool estimate_eye_pose(Eigen::Quaterniond& rot, const cv::Mat& image, const Eigen::Quaterniond& head_rot, const cv::Mat_<float>& detected_landmarks, bool left);

#endif // ifndef EYE_ROTATION_H


