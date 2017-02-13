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

#define DEFAULT_RES_FACTOR  2
#define DEFAULT_ALPHA       0.7f
#define DEFAULT_BETA        1.0f
#define DEFAULT_LAMBDA      0.04f
#define DEFAULT_P           2
#define DEFAULT_ITERATIONS  20
#define DEFAULT_PADDING     2

// minimum value for all float parameters
#define EPS 0.000001f


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

        resolutionFactor = DEFAULT_RES_FACTOR;

        pointSpreadFunction = new GaussianPointSpreadFunction(3, 1.0f);
        pointSpreadFunction->createKernel();

        alpha = DEFAULT_ALPHA;
        beta = DEFAULT_BETA;
        lambda = DEFAULT_LAMBDA;
        p = DEFAULT_P;
        maxIterations = DEFAULT_ITERATIONS;
        padding = DEFAULT_PADDING;
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

    void setResolutionFactor(uint _resolutionFactor){
        if (_resolutionFactor == 0) {
            resolutionFactor = DEFAULT_RES_FACTOR;
        }
        else {
            resolutionFactor = _resolutionFactor;
        }
    }

    void setAlpha(float _alpha){
        if (_alpha <= EPS) {
            alpha = DEFAULT_ALPHA;
        }
        else {
            alpha = _alpha;
        }
    }

    void setBeta(float _beta){
        if (_beta <= EPS) {
            beta = DEFAULT_BETA;
        }
        else {
            beta = _beta;
        }
    }

    void setLambda(float _lambda){
        if (_lambda <= EPS) {
            lambda = DEFAULT_LAMBDA;
        }
        else {
            lambda = _lambda;
        }
    }

    void setP(int _p){
        if (p <= 0) {
            p = DEFAULT_P;
        }
        else {
            p = _p;
        }
    }

    void setMaxIterations(uint _maxIterations) {
        if (_maxIterations == 0) {
            maxIterations = DEFAULT_ITERATIONS;
        }
        else {
            maxIterations = _maxIterations;
        }
    }

    void setPadding(uint _padding){
        padding = _padding;
    }

};

#endif //SUPER_RES_PARAMETERS_H
