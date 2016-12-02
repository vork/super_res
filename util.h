//
// Created by Paul Sanzenbacher on 02.12.16
//

#ifndef SUPER_RES_UTIL_H
#define SUPER_RES_UTIL_H


#include <opencv2/core/types.hpp>

// convert a list of float points to discrete points by rounding the components
std::vector<cv::Point> roundPoints(std::vector<cv::Point2f> &fpoints) {
    std::vector<cv::Point> roundedPoints;
    for (cv::Point2f fpoint : fpoints) {
        roundedPoints.push_back(cv::Point((int)roundf(fpoint.x), (int)roundf(fpoint.y)));
    }
    return roundedPoints;
}



#endif //SUPER_RES_UTIL_H
