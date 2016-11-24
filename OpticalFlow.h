//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_OPTICALFLOW_H
#define SUPER_RES_OPTICALFLOW_H

#include "opencv2/core.hpp"
#include "opencv2/optflow.hpp"
#include "opencv2/video/tracking.hpp"

using namespace cv;
using namespace std;

/// Class which executes the optical flow implementation of opencv
class OpticalFlow {
    void calculateOpticalFlow(vector<Mat> lowResImg, vector<Point2f> &points);
};


#endif //SUPER_RES_OPTICALFLOW_H
