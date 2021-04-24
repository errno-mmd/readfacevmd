// -*- C++ -*-

#ifndef MY_GAZE_ESTIMATION_H
#define MY_GAZE_ESTIMATION_H

#include <LandmarkDetectorModel.h>
#include <opencv2/core/core.hpp>

namespace MyGazeEstimation
{
    void EstimateGaze(const LandmarkDetector::CLNF& clnf_model, cv::Point3f& gaze_absolute, float fx, float fy, float cx, float cy, bool left_eye);
}

#endif // ifndef MY_GAZE_ESTIMATION_H

