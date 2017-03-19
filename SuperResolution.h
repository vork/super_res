//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_SUPERRESOLUTION_H
#define SUPER_RES_SUPERRESOLUTION_H

#include "Parameters.h"
#include "ImageSet.h"
#include "Timer.h"


struct MedianEstimationResult {
    cv::Mat1f medianEstimate;
    cv::Mat1f sqrtContributions;

    MedianEstimationResult(const cv::Mat1f &_medianEstimate, const cv::Mat1f &_sqrtContributions) {
        medianEstimate = _medianEstimate;
        sqrtContributions = _sqrtContributions;
    }
};

typedef std::vector<cv::Point2f> PointList;

/**
 * Wrapper class for the superresolution algorithm
 */

class SuperResolution {

protected:

    bool computeColoredResult;

    Parameters * parameters;

    cv::Size lrSize;
    cv::Size hrSize;

    // sqrtContributions corresponds to matrix A in fast gradient back project (see Farsiu 04 paper)
    cv::Mat1f sqrtContributions;

    // initial estimated solution
    cv::Mat1f initialSolution;

    // current high resolution image
    cv::Mat3f hrImage;

    cv::Mat1f gradientBackProject();
    cv::Mat1f gradientRegulization();

    // create timer for runtime analysis
    Timer timer;

    std::function<void(cv::Mat1f intermediateResult)> iterationCallback;

    MedianEstimationResult medianEstimation(ImageSet * imageSet, PointList offsets);

public:
    SuperResolution(Parameters *parameters, bool _computeColoredResult);

    // compute super-resolution image
    cv::Mat3f compute();

    // Extract the color information from the low res images and apply them in L*a*b space
//    cv::Mat extractColorInformation();

    // compute super-resolution when initial solution and sqrtContributions are known
    cv::Mat1f computeWithInitialSolutionAndSqrtContributions(cv::Mat1f _initialSolution, cv::Mat1f _sqrtContributions);

    void setIterationCallback(const std::function<void(cv::Mat1f)> &iterationCallback);




};


#endif //SUPER_RES_SUPERRESOLUTION_H
