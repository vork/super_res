//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_GRADIENTUTILS_H
#define SUPER_RES_GRADIENTUTILS_H

#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

class GradientUtils {
    //TODO howto store the psf
    void GradientBackProjection(Mat curEstimation, Mat orgEstimation, float normFactor);
    void GradientRegulization(Mat curEstimation, int spatialWidth, int spatialHeight, float alpha);
};


#endif //SUPER_RES_GRADIENTUTILS_H
