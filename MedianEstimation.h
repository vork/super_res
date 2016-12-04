//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_MEDIANESTIMATION_H
#define SUPER_RES_MEDIANESTIMATION_H

static const int padding = 2;

#include "opencv2/core.hpp"
#include "ImageSet.h"
#include "Parameters.h"


/// Executes the initial Median blurred high res version for the fast and robust
/// super resolution
class MedianEstimation {

protected:
    Parameters * parameters;
    std::vector<cv::Point2f> offsets;

    /* available after calling computeEstimatedSuperResolution()
     * corresponds to matrix A in fast gradient back project (see Farsiu 04 paper) */
    cv::Mat1f sqrtContributions;

public:

    MedianEstimation(Parameters *parameters, const std::vector<cv::Point2f> &offsets);

    // calculate median shift estimate with
    cv::Mat1f computeEstimatedSuperResolution(cv::Mat1f &sqrtContributions);

    const cv::Mat1f &getSqrtContributions() const;

};


#endif //SUPER_RES_MEDIANESTIMATION_H
