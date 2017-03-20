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
    // low resolution images (RGB, normalized)
    std::vector<cv::Mat3f> lowResolutionImages;

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

    // TODO: what is the ref image?
//    cv::Mat refImage;

    // default constructor
public:
    void createParameters(uint resolutionFactor, float alpha, float beta, float lambda, int p, uint maxIterations, uint padding, std::vector<cv::Mat3f> _lowResolutionImages) {
        this->resolutionFactor = resolutionFactor;
        this->alpha = alpha;
        this->beta = beta;
        this->lambda = lambda;
        this->p = p;
        this->maxIterations = maxIterations;
        this->padding = padding;

        setLowResolutionImages(_lowResolutionImages);
        pointSpreadFunction = new GaussianPointSpreadFunction(3, 1.0f);
        pointSpreadFunction->createKernel();
//        refImage = referenceImage;
    }

    const std::vector<cv::Mat3f> &getLowResolutionImages() const {
        return lowResolutionImages;

    }

    void setLowResolutionImages(const std::vector<cv::Mat3f> &lowResolutionImages) {
        Parameters::lowResolutionImages = lowResolutionImages;
        assert(lowResolutionImages.size() > 0);
        assert(resolutionFactor > 0);
        lowResSize = lowResolutionImages[0].size();
        highResSize = cv::Size(lowResSize.width * resolutionFactor + 1, lowResSize.height * resolutionFactor + 1);
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

    std::string str() {

        std::stringstream stream;

        stream << "resolution factor: " << resolutionFactor << "\n";
        stream << "alpha: " << alpha << "\n";
        stream << "beta: " << beta << "\n";
        stream << "lambda: " << lambda << "\n";
        stream << "p: " << p << "\n";
        stream << "max iterations: " << maxIterations << "\n";
        stream << "padding: " << padding << "\n";
        stream << "num images: " << lowResolutionImages.size() << "\n";

        return stream.str();
    }
};

class ParameterFactory {
private:
    uint resolutionFactor = DEFAULT_RES_FACTOR;     // resolution improvement factor
    float alpha = DEFAULT_ALPHA;
    float beta = DEFAULT_BETA;
    float lambda = DEFAULT_LAMBDA;
    int p = DEFAULT_P;                      // gradient regularization radius
    uint maxIterations = DEFAULT_ITERATIONS;
    uint padding = DEFAULT_PADDING; // crop lr images by padding to avoid black borders due to offsets

public:
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

    Parameters* build(std::vector<cv::Mat3f> _lowResolutionImages) {
        Parameters* params = new Parameters();
        params->createParameters(resolutionFactor, alpha, beta, lambda, p, maxIterations, padding, _lowResolutionImages);
        return params;
    }
};

#endif //SUPER_RES_PARAMETERS_H
