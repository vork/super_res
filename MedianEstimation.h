//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_MEDIANESTIMATION_H
#define SUPER_RES_MEDIANESTIMATION_H

#include "opencv2/core.hpp"
#include "ImageSet.h"
#include "Parameters.h"


/// Executes the initial Median blurred high res version for the fast and robust
/// super resolution
class MedianEstimation {

protected:
    ImageSet * imageSet;
    Parameters * parameters;
    std::vector<cv::Point2f> offsets;

public:

    MedianEstimation(ImageSet *imageSet, Parameters *parameters, const std::vector<cv::Point2f> &offsets);

    cv::Mat1f computeEstimatedSuperResolution();

};


#endif //SUPER_RES_MEDIANESTIMATION_H
