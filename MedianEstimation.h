//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_MEDIANESTIMATION_H
#define SUPER_RES_MEDIANESTIMATION_H

#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

/// Executes the initial Median blurred high res version for the fast and robust
/// super resolution
class MedianEstimation {
public:
    void blurredHighResEstimation(InputArrayOfArrays lowRes, vector<Point2f> displacement,
                                  int hrwidth, int hrheight, float scalefactor, Mat &estimate,
                                  Mat &normFactor);
};


#endif //SUPER_RES_MEDIANESTIMATION_H
