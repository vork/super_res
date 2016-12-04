//
// Created by Paul Sanzenbacher on 30.11.16
//

#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>
#include "SuperResolution.h"
#include "MedianEstimation.h"
#include "OpticalFlow.h"
#include "util.h"

using namespace cv;
using namespace std;

SuperResolution::SuperResolution(Parameters *parameters) {
    this->parameters = parameters;
}

cv::Mat1f SuperResolution::gradientBackProject() {

    // back projected matrix
    Mat1f back;

    // store point spread function
    PointSpreadFunction * psf = parameters->getPointSpreadFunction();

    // blur current solution using the point spread function
    const int ddepth = -1; // use same depth for dst image
    const Point anchorPoint = Point(-1, -1); // use center as anchor point
    const double delta = 0.0; // OpenCV default
    const int borderType = BORDER_REFLECT;
    Mat1f kernel = psf->getKernel();
    filter2D(hrImage, back, ddepth, kernel, anchorPoint, delta, borderType);

    // compute difference between blurred current solution and initial solution
     back = back - initialSolution;

    // weight with sqrtContributions
     back = sqrtContributions.mul(back);

    // compute sign for each value
    int numPixels = parameters->getHighResSize().width * parameters->getHighResSize().height;
    float * backData = (float *)back.data;
    for (int i = 0; i < numPixels; i++) {
        backData[i] = signf(backData[i]);
    }

    // unblur backprojected image
    PointSpreadFunction * flippedPSF = psf->flipped();
    kernel = flippedPSF->getKernel();

    Mat1f unfiltered;
    back = sqrtContributions.mul(back);
    filter2D(back, unfiltered, ddepth, kernel, anchorPoint, delta, borderType);
    delete flippedPSF;

    return unfiltered;
}

cv::Mat1f SuperResolution::gradientRegulization() {

    // get window radius
    const int p = parameters->getP();

    // get constant alpha from parameter set
    const float alpha = parameters->getAlpha();

    // regulized matrix
    Mat1f reg(parameters->getHighResSize(), 0.0f);

    // create mirror padded version of current solution
    Mat1f padded;
    copyMakeBorder(hrImage, padded, p, p, p, p, BORDER_REFLECT);

    Mat1f shifted;

    // iterate over window
    for (int l = -p; l <= p; l++) {
        for (int m = -p; m <= p; m++) {

            Rect roi(Point(p - l, p - m), parameters->getHighResSize());

            Mat1f shift = padded(roi);

            Mat1f diff = hrImage - shift;

            // compute sign
            Mat1f sign(parameters->getHighResSize());
            int numPixels = parameters->getHighResSize().width * parameters->getHighResSize().height;
            float * signData = (float *)sign.data;
            float * diffData = (float *)diff.data;
            for (int i = 0; i < numPixels; i++) {
                signData[i] = (float)sgn<float>(diffData[i]);
            }

            Mat1f signPadded;
            copyMakeBorder(sign, signPadded, p, p, p, p, BORDER_CONSTANT, 0.0f);

            shift = signPadded(Rect(Point(p + l, p + m), parameters->getHighResSize()));

            diff = sign - shift;

            float factor = powern(alpha, abs(l) + abs(m));

            reg = reg + factor * diff;
        }
    }
    return reg;
}

Mat1f SuperResolution::compute() {

    timer.reset();

    unique_ptr<OpticalFlow>opticalFlow(new OpticalFlow());
    vector<Point2f> imageFlows = opticalFlow->computeOffsetsForImageSet(parameters->getImageSet());

    // compute offsets as negative image flow
    vector<Point2f> offsets;
    for (Point2f imageFlow : imageFlows) {
        offsets.push_back((-1) * imageFlow);
    }

    timer.printTimeAndReset("offset computation");

    // compute median estimation
    unique_ptr<MedianEstimation> medianEstimation(new MedianEstimation(parameters, offsets));

    // compute initial solution with median shift method
    hrImage = medianEstimation->computeEstimatedSuperResolution(sqrtContributions);
    timer.printTimeAndReset("median estimation");

    hrImage = computeWithInitialSolutionAndSqrtContributions(hrImage, sqrtContributions);

    return hrImage;
}

Mat1f SuperResolution::computeWithInitialSolutionAndSqrtContributions(Mat1f _initialSolution, Mat1f _sqrtContributions) {

    timer.reset();

    initialSolution = _initialSolution.clone();
    hrImage = _initialSolution;
    sqrtContributions = _sqrtContributions;


    const float beta = parameters->getBeta();
    const float lambda = parameters->getLambda();
    Mat1f gradient(parameters->getHighResSize());

    for (unsigned int iteration = 0; iteration < parameters->getMaxIterations(); iteration++) {

        // gradient back projection
        Mat1f backProjected = gradientBackProject();

        Mat1f regularized = gradientRegulization();

        // compute gradient
         gradient = backProjected + (lambda * regularized);

        // subtract gradient
        hrImage = hrImage - (beta * gradient);

        timer.setMarker();
    }

    timer.printTimeAndReset("gradient descent");

    return hrImage;
}