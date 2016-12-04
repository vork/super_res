//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_PARAMETERS_H
#define SUPER_RES_PARAMETERS_H


#include <opencv2/core/mat.hpp>
#include <assert.h>
#include "PointSpreadFunction.h"
#include <iostream>
#include "ImageSet.h"

typedef unsigned int uint;

/**
 * parameter class that holds together all parameters that are necessary to compute a super-resolution image
 */

class Parameters {

protected:

    ImageSet * imageSet;

    cv::Size lowResSize;       // size of the low resolution image
    cv::Size highResSize;
    uint resolutionFactor;     // resolution improvement factor
    PointSpreadFunction * pointSpreadFunction;
    float alpha;
    float beta;
    float lambda;
    int p;                      // gradient regularization radius
    uint maxIterations;
    uint padding; // crop lr images by padding to avoid black borders due to offsets

public:

    // default constructor
    Parameters(ImageSet * _imageSet) {

        setImageSet(_imageSet);

        // set default parameters

        resolutionFactor = 2;

        pointSpreadFunction = new GaussianPointSpreadFunction(3, 1.0f);
        pointSpreadFunction->createKernel();

        alpha = 0.7f;
        beta = 1.0f;
        lambda = 0.04f;
        p = 2;
        maxIterations = 20;
        padding = 2;
    }

    void setImageSet(ImageSet *imageSet) {
        Parameters::imageSet = imageSet;
        lowResSize = imageSet->getImageSize();
        highResSize = cv::Size(lowResSize.width * resolutionFactor + 1, lowResSize.height * resolutionFactor + 1);
    }

    ImageSet *getImageSet() const {
        return imageSet;
    }

    const cv::Size &getLowResSize() const {
        return lowResSize;
    }

    cv::Size &getHighResSize() {
        return highResSize;
    }

    uint getResolutionFactor() const {
        return resolutionFactor;
    }

    PointSpreadFunction *getPointSpreadFunction() const {
        return pointSpreadFunction;
    }

    float getAlpha() const {
        return alpha;
    }

    float getBeta() const {
        return beta;
    }

    float getLambda() const {
        return lambda;
    }

    int getP() const {
        return p;
    }

    uint getMaxIterations() const {
        return maxIterations;
    }

    uint getPadding() const {
        return padding;
    }

};

#endif //SUPER_RES_PARAMETERS_H
