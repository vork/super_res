//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_PARAMETERS_H
#define SUPER_RES_PARAMETERS_H


#include <opencv2/core/mat.hpp>
#include <assert.h>

typedef unsigned int uint;

class Parameters {

protected:
    cv::Size lowResSize;       // size of the low resolution image
    uint resolutionFactor;     // resolution improvement factor
    uint psfKernelSize;        // gaussian kernel size (must be odd)
    float psfSigma;            // gaussian kernel sigma
    float alpha;
    float beta;
    float lambda;
    int p;
    uint maxIterations;

public:

    // default constructor
    Parameters(cv::Size lowResSize) {
        // set default parameters
        this->lowResSize = lowResSize;
        resolutionFactor = 2;
        psfKernelSize = 3;
        psfSigma = 1.0f;
        alpha = 0.7f;
        beta = 1.0f;
        lambda = 0.04f;
        p = 2;
        maxIterations = 25;
    }

    Parameters(const cv::Size &lowResSize, uint resolutionFactor, uint psfKernelSize, float psfSigma, float alpha,
               float beta, float lambda, int p, uint maxIterations) : lowResSize(lowResSize),
                                                                      resolutionFactor(resolutionFactor),
                                                                      psfKernelSize(psfKernelSize), psfSigma(psfSigma),
                                                                      alpha(alpha), beta(beta), lambda(lambda), p(p),
                                                                      maxIterations(maxIterations) {
        // enforce odd kernel size
        assert(psfKernelSize % 2 == 1);
    }

    const cv::Size &getLowResSize() const {
        return lowResSize;
    }

    cv::Size calcHighResSize() {
        return cv::Size(lowResSize.width * resolutionFactor, lowResSize.height * resolutionFactor);
    }

    uint getResolutionFactor() const {
        return resolutionFactor;
    }

    uint getPsfKernelSize() const {
        return psfKernelSize;
    }

    float getPsfSigma() const {
        return psfSigma;
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
