//
// Created by Paul Sanzenbacher on 03.12.16
//

#ifndef SUPER_RES_POINTSPREADFUNCTION_H
#define SUPER_RES_POINTSPREADFUNCTION_H

#include <assert.h>
#include <opencv2/core.hpp>
#include <iostream>

/**
 * Abstract class for point spread functions
 */

class PointSpreadFunction {

protected:
    cv::Size kernelSize;
    cv::Mat1f kernel;

public:
    PointSpreadFunction(const cv::Size kernelSize) : kernelSize(kernelSize) {}

    const cv::Mat1f &getKernel() const {
        return kernel;
    }

    virtual void createKernel() = 0;

    // create a kernel that is rotated by 180 degrees (or flipped on x and y axis)
    virtual PointSpreadFunction * flipped() = 0;

    cv::Point getAnchorPoint() {
        return cv::Point(kernelSize.width / 2, kernelSize.height / 2);
    }

};

/**
 * PointSpreadFunction implementation for gaussian filter
 */

class GaussianPointSpreadFunction : public PointSpreadFunction {
protected:
    float sigma;
    int kernelDim;

    float gauss(float x) {
        return expf( -(x * x) / (2 * sigma * sigma)) / (sigma * sqrtf(2 * M_PI));
    }


public:
    GaussianPointSpreadFunction(int kernelDim, float sigma) : PointSpreadFunction(cv::Size(kernelDim, kernelDim)), kernelDim(kernelDim), sigma(sigma)  {
        // enforce odd and quadratic kernel size
        assert(kernelDim % 2 == 1);
    }

    void createKernel() override {
        // compute gauss kernel
        int kernelRadius = kernelDim / 2;

        kernel = cv::Mat1f(kernelSize);

        // store sum of all kernel values for normalization
        float kernelSum = 0.0f;
        for (int y = 0; y < kernelDim; y++) {
            float * kernelRowPtr = kernel.ptr<float>(y);
            for (int x = 0; x < kernelDim; x++) {
                float h = hypotf(x - kernelRadius, y - kernelRadius);
                float g = gauss(h);
                kernelRowPtr[x] = g;
                kernelSum += g;
            }
        }

        // normalize kernel
        kernel /= kernelSum;
    }

    GaussianPointSpreadFunction * flipped() {
        GaussianPointSpreadFunction * fl = new GaussianPointSpreadFunction(kernelDim, sigma);

        // gaussian psf is point symmetric
        fl->createKernel();

        return fl;
    }

};


#endif //SUPER_RES_POINTSPREADFUNCTION_H
