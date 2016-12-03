//
// Created by Paul Sanzenbacher on 30.11.16
//

#include <opencv2/imgcodecs.hpp>
#include "SuperResolution.h"
#include "MedianEstimation.h"
#include "Timer.h"
#include "OpticalFlow.h"

using namespace cv;
using namespace std;

SuperResolution::SuperResolution(Parameters *parameters, ImageSet *imageSet) {
    this->parameters = parameters;
    this->imageSet = imageSet;
}

Mat1f SuperResolution::compute() {

    // create timer for runtime analysis
    Timer timer;


    unique_ptr<OpticalFlow>opticalFlow(new OpticalFlow());
    vector<Point2f> offsets = opticalFlow->computeOffsetsForImageSet(imageSet);
    timer.printTimeAndReset("offset computation");

    // compute median estimation
    unique_ptr<MedianEstimation> medianEstimation(new MedianEstimation(imageSet, parameters, offsets));

    // compute initial solution with median shift method
    Mat1f hrImage = medianEstimation->computeEstimatedSuperResolution();
    timer.printTimeAndReset("median estimation");

    // save estimate for debugging
    imwrite("estimate.png", hrImage * 255);


    for (unsigned int iteration = 0; iteration < parameters->getMaxIterations(); iteration++) {
        // TODO: ...
    }


    return hrImage;
}