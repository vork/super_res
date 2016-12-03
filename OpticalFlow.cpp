//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include <cv.hpp>
#include "OpticalFlow.h"

using namespace cv;
using namespace std;


vector<Point2f> OpticalFlow::computeOffsetsForImageSet(ImageSet *imageSet) {

    // store offsets for each image relative to first image ( -> first entry is zero point )
    vector<Point2f> offsets;

    // make sure image set is in its iniital state
    if (!imageSet->isInitialState()) {
        imageSet->reset();
    }

    // first image has zero offset by default
    offsets.push_back(Point2f(0, 0));

    // set first image as reference image
    Mat1f referencef = imageSet->next();
    Mat1b reference;
    referencef.convertTo(reference, CV_8UC1, 255);

    // TODO: region of interest?

    // find good features in reference image
    const int maxCorners = 100;
    vector<Point2f> corners;
    const double qualityLevel = 0.3;
    const double minDistance = 7.0;
    const double blockSize = 7.0;
    goodFeaturesToTrack(reference, corners, maxCorners, qualityLevel, minDistance);

    if (corners.size() == 0) {
        cout << "no good features found. maybe you have to adjust the parameters." << endl;

        // return empty vector
        return offsets;
    }


    // iterate over all other images
    for (int i = 1; i < imageSet->getNumImages(); i++) {

        Mat1f imagef = imageSet->next();
        Mat1b image;
        imagef.convertTo(image, CV_8UC1, 255);

        vector<Point2f> nextCorners(corners.size());

        // default values and suggested parameters from documentation
        vector<uchar> status;
        vector<float> err;
        Size winSize(21, 21);
        int maxLevel = 3;
        TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS, 20, 0.03);
        int flags = 0;
        double minEigThreshold = 1e-4;

        // compute optical flow
        calcOpticalFlowPyrLK(reference, image, corners, nextCorners, status, err, winSize, maxLevel, termcrit, flags, minEigThreshold);

        // compute the average of relative offsets
        Point2f relativeAverageOffset(0.0f, 0.0f);
        for (int j = 0; j < corners.size(); j++) {
            Point2f cornerOffset = nextCorners[j] - corners[j];
            relativeAverageOffset += cornerOffset;
        }
        relativeAverageOffset /= (float)corners.size();

        // add to offsets list
        offsets.push_back(relativeAverageOffset);
    }
    return offsets;
}
