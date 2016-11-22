//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_MEDIANESTIMATION_H
#define SUPER_RES_MEDIANESTIMATION_H

#include "opencv2/core.hpp"

using namespace cv;

/// Executes the initial Median blurred high res version for the fast and robust
/// super resolution
class MedianEstimation {
public:
    void blurredHighResEstimation(InputArrayOfArrays lowRes, );
};


#endif //SUPER_RES_MEDIANESTIMATION_H
