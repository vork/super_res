//
// Created by Paul Sanzenbacher on 02.12.16
//

#ifndef SUPER_RES_UTIL_H
#define SUPER_RES_UTIL_H

#include <opencv2/core/core.hpp>

#include <OpenGL/OpenGL.h>

// sign function

float signf(float val);

template <typename T> int sgn(T val) {
    if (val > T(0)) {
        return 1;
    }
    else if (val < T(0)) {
        return -1;
    }
    return 0;
}

// euclidian modulo
int mod(int a, int m);

// convert a list of float points to discrete points by rounding the components
std::vector<cv::Point> roundPoints(std::vector<cv::Point2f> &fpoints);

int partition(float * a, int l, int r);

void quickSort( float * a, int l, int r);

float medianOfFloatArray(float * a, unsigned int n);

// power function with natural exponent (square and multiply)
float powern(float a, unsigned int n);


cv::Mat1f readMat1fFromFile(std::string filename, cv::Size size);

void showMatrixRegion(cv::Mat1f mat, cv::Rect region);

cv::Mat1f alignImages(std::vector<cv::Mat1f> images);

cv::Mat1f shiftImage(cv::Mat1f image, cv::Point offset, int padding = 0, float fill = 0.0f);

/**
 * For a 3-channel image, determine whether it uses colors or not
 */
bool isGrayscaleImage(cv::Mat3b image);

cv::Mat3f convertToYCrCb(cv::Mat3f rgbImage);
cv::Mat3f convertToRGB(cv::Mat3f yCrCbImage);

#endif SUPER_RES_UTIL_H
