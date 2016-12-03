//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_PARAMETERS_H
#define SUPER_RES_PARAMETERS_H


#include <opencv2/core/mat.hpp>
#include <assert.h>
#include "PointSpreadFunction.h"
#include <iostream>

typedef unsigned int uint;

class Parameters {

protected:
    cv::Size lowResSize;       // size of the low resolution image
    uint resolutionFactor;     // resolution improvement factor
    PointSpreadFunction * pointSpreadFunction;
    float alpha;
    float beta;
    float lambda;
    int p;                      // gradient regularization radius
    uint maxIterations;

public:

    // default constructor
    Parameters(cv::Size lowResSize) {
        // set default parameters
        this->lowResSize = lowResSize;
        resolutionFactor = 2;

        pointSpreadFunction = new GaussianPointSpreadFunction(3, 1.0f);
        pointSpreadFunction->createKernel();

        alpha = 0.7f;
        beta = 1.0f;
        lambda = 0.04f;
        p = 2;
        maxIterations = 25;
    }

    Parameters(cv::Size lowResSize, uint resolutionFactor, PointSpreadFunction *pointSpreadFunction, float alpha,
               float beta, float lambda, int p, uint maxIterations) : lowResSize(lowResSize),
                                                                      resolutionFactor(resolutionFactor),
                                                                      pointSpreadFunction(pointSpreadFunction),
                                                                      alpha(alpha), beta(beta), lambda(lambda), p(p),
                                                                      maxIterations(maxIterations) {}

    const cv::Size &getLowResSize() const {
        return lowResSize;
    }

    cv::Size calcHighResSize() {
        return cv::Size(lowResSize.width * resolutionFactor, lowResSize.height * resolutionFactor);
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

};

#endif //SUPER_RES_PARAMETERS_H
