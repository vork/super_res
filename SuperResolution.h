//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_SUPERRESOLUTION_H
#define SUPER_RES_SUPERRESOLUTION_H

#include "Parameters.h"
#include "ImageSet.h"

/**
 * Wrapper class for the superresolution algorithm
 */


class SuperResolution {

protected:
    Parameters * parameters;
    ImageSet * imageSet;
    std::vector<cv::Point2f> offsets;


public:
    SuperResolution(Parameters *parameters, ImageSet *imageSet);
    cv::Mat1f compute();
};


#endif //SUPER_RES_SUPERRESOLUTION_H
