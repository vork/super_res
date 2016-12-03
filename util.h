//
// Created by Paul Sanzenbacher on 02.12.16
//

#ifndef SUPER_RES_UTIL_H
#define SUPER_RES_UTIL_H

#include <opencv2/core/types.hpp>

// sign function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// convert a list of float points to discrete points by rounding the components
std::vector<cv::Point> roundPoints(std::vector<cv::Point2f> &fpoints);

int partition(float * a, int l, int r);

void quickSort( float * a, int l, int r);

float medianOfFloatArray(float * a, unsigned int n);

// power function with natural exponent (square and multiply)
float powern(float a, unsigned int n);


#endif SUPER_RES_UTIL_H
