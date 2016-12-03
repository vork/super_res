//
// Created by Paul Sanzenbacher on 02.12.16
//

#ifndef SUPER_RES_UTIL_H
#define SUPER_RES_UTIL_H


#include <opencv2/core/types.hpp>
#include "ImageSet.h"

// convert a list of float points to discrete points by rounding the components
std::vector<cv::Point> roundPoints(std::vector<cv::Point2f> &fpoints) {
    std::vector<cv::Point> roundedPoints;
    for (cv::Point2f fpoint : fpoints) {
        roundedPoints.push_back(cv::Point((int)roundf(fpoint.x), (int)roundf(fpoint.y)));
    }
    return roundedPoints;
}

int partition(float * a, int l, int r) {
    float pivot = a[l], t;
    int i = l, j = r + 1;

    while( 1)
    {
        do ++i; while( a[i] <= pivot && i <= r );
        do --j; while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
    }
    t = a[l]; a[l] = a[j]; a[j] = t;
    return j;
}

void quickSort( float * a, int l, int r)
{
    int j;

    if( l < r )
    {
        // divide and conquer
        j = partition( a, l, r);
        quickSort( a, l, j-1);
        quickSort( a, j+1, r);
    }

}








//void quickSort(float * a, int left, int right) {
//
//    int i = left, j = right;
//    float tmp;
//    float pivot = a[(left + right) / 2];
//
//    // partition
//    while (i <= j) {
//
//        while (a[i] < pivot) {
//            i++;
//        }
//
//        while (a[j] > pivot) {
//            j--;
//        }
//
//        if (i <= j) {
//            tmp = a[i];
//            a[i] = a[j];
//            a[j] = tmp;
//            i++;
//            j--;
//        }
//    };
//
//    // recursion
//    if (left < j) {
//        quickSort(a, left, j);
//    }
//
//    if (i < right) {
//        quickSort(a, i, right);
//    }
//}

float medianOfFloatArray(float * a, unsigned int n) {
    if (n > 2) {
        quickSort(a, 0, n - 1);
    }
    if (n % 2 == 0) {
        return 0.5f * (a[n / 2] + a[n / 2 - 1]);
    }
    else {
        return a[n / 2];
    }
}


#endif SUPER_RES_UTIL_H
