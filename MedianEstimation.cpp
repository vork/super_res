//
// Created by Benedikt Boss on 22/11/2016.
//

#include "MedianEstimation.h"


using namespace cv;
using namespace std;


MedianEstimation::MedianEstimation(ImageSet *imageSet, Parameters *parameters, const vector<Point> &discreteOffsets)
        : imageSet(imageSet), parameters(parameters), discreteOffsets(discreteOffsets) {}

cv::Mat1f MedianEstimation::computeEstimatedSuperResolution() {

    Size lrSize = parameters->getLowResSize();
    Size hrSize = parameters->calcHighResSize();
    int resolutionFactor = parameters->getResolutionFactor();

    // estimated high resolution image
    Mat1f estimate(hrSize, 0.0f);




}
