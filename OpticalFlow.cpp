//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include <cv.hpp>
#include "OpticalFlow.h"
#include "util.h"

using namespace cv;
using namespace std;

#define VISUALIZE false



vector<Point2f> OpticalFlow::computeOffsetsForImageSet(ImageSet *imageSet) {

    // store offsets for each image relative to first image ( -> first entry is zero point )
    vector<Point2f> offsets;

    // make sure image set is in its initial state
    if (!imageSet->isInitialState()) {
        imageSet->reset();
    }

    // first image has zero offset by default
    offsets.push_back(Point2f(0, 0));

    // set first image as reference image
    Mat1f referencef = imageSet->next();
    Mat1b reference;
    referencef.convertTo(reference, CV_8UC1);

    // TODO: region of interest?


    // find good features in reference image
    const int maxCorners = 500;
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
        imagef.convertTo(image, CV_8UC1);

        vector<Point2f> nextCorners(corners.size());

        // default values and suggested parameters from documentation
        vector<uchar> status;
        vector<float> err;
        Size winSize(21, 21);
        int maxLevel = 15;
        TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS, 20, 0.03);
        int flags = 0;
        double minEigThreshold = 1e-4;

        // compute optical flow
        calcOpticalFlowPyrLK(reference, image, corners, nextCorners, status, err, winSize, maxLevel, termcrit, flags, minEigThreshold);


        vector<Point2f> cornerOffsets(corners.size());
        for (int j = 0; j < corners.size(); j++) {
            cornerOffsets[j] = nextCorners[j] - corners[j];
        }

        // filter corners (remove outliers)
        bool offsetFilterSuccess[cornerOffsets.size()];
        for (int j = 0; j < cornerOffsets.size(); j++) {
            offsetFilterSuccess[j] = false;
        }
        cornerOffsets = filterOffsets(cornerOffsets, offsetFilterSuccess);

        if (cornerOffsets.size() == 0) {
            cout << "warning: no offsets left after filtering!" << endl;
        }

        // compute the average of relative offsets
        Point2f relativeAverageOffset(0.0f, 0.0f);
        for (int j = 0; j < cornerOffsets.size(); j++) {
            relativeAverageOffset += cornerOffsets[j];
        }
        relativeAverageOffset /= (float)cornerOffsets.size();

        // add to offsets list
        offsets.push_back(relativeAverageOffset);

#if VISUALIZE

        Mat1f fadedGray;
        addWeighted(reference, 0.5, image, 0.5, 0, fadedGray);
        Mat3f faded;
        cvtColor(fadedGray, faded, CV_GRAY2BGR);
        faded = faded / 255;
        for (int j = 0; j < corners.size(); j++) {
            Vec3f lineColor = Vec3f(1.0f, 1.0f, 0);
            if (!offsetFilterSuccess[j]) {
                lineColor = Vec3f(0.0f, 0.0f, 1.0f);
            }
            line(faded, corners[j], nextCorners[j], lineColor, 5);
        }
        Size newSize = faded.size() / 5;
        resize(faded, faded, newSize);
        imshow("vis", faded);
        waitKey(0);

        Mat1f imageShift = shiftImage(image, -relativeAverageOffset);
        addWeighted(reference, 0.5, imageShift, 0.5, 0, fadedGray);
        cvtColor(fadedGray, faded, CV_GRAY2BGR);
        faded = faded / 255;
        resize(faded, faded, newSize);
        imshow("vis", faded);
        waitKey(0);

#endif


    }
    return offsets;
}

vector<Point2f> OpticalFlow::filterOffsets(vector<Point2f> offsets, bool * success) {

    vector<Point2f> filteredOffsets;

//    float angles[offsets.size()], lengths[offsets.size()];
//    for (int i = 0; i < offsets.size(); i++) {
//        Point2f p = offsets[i];
//        angles[i] = atan2f(p.y, p.x);
//        lengths[i] = hypotf(p.y, p.x);
//    }
//
//
//    const float medianAngle = medianOfFloatArray(angles, offsets.size());
//    const float medianLength = medianOfFloatArray(lengths, offsets.size());
//
//    const float maxAbsoluteAngleDifference = (2 * M_PI) / 18;
//    const float minRelativeLengthDifference = 0.9f;
//
//    cout << medianAngle << " " << medianLength << endl;
//
//    for (int i = 0; i < offsets.size(); i++) {
//        float angleDist = fmodf(fabsf(angles[i] - medianAngle), 2.0f * M_PI);
//        if (angleDist <= maxAbsoluteAngleDifference) {
//            float lengthDist = fminf(medianLength / lengths[i], lengths[i] / medianLength);
//            if (lengthDist >= minRelativeLengthDifference) {
//                filteredOffsets.push_back(offsets[i]);
//                success[i] = true;
//            }
//        }
//    }


    // calculate median x and y offset
    int numOffsets = offsets.size();
    float xvals[numOffsets], yvals[numOffsets];
    for (int j = 0; j < offsets.size(); j++) {
        Point2f offset = offsets[j];
        xvals[j] = offset.x;
        yvals[j] = offset.y;
    }
    float xmedian = medianOfFloatArray(xvals, numOffsets);
    float ymedian = medianOfFloatArray(yvals, numOffsets);

    // TODO: set relative to image size
    float tolerance = 3.0f;

    int i = 0;
    for (int j = 0; j < numOffsets; j++) {
        Point2f diff = offsets[j] - Point2f(xmedian, ymedian);
        if (fabsf(diff.x) <= tolerance && fabsf(diff.y) <= tolerance) {
            filteredOffsets.push_back(offsets[j]);
            success[j] = true;
            i++;
        }
    }

    cout << xmedian << " " << ymedian << "  " << i << "/" << offsets.size() << endl;

//    cout << xmedian << " " << ymedian << endl;
//
//
//    // filtering
//    int binsPerDimension = 10;
//    float binSizeX = (xmedian * 2) / binsPerDimension;
//    float binSizeY = (ymedian * 2) / binsPerDimension;
//    Mat1i bins(Size(binsPerDimension, binsPerDimension), 0);
//
//    vector<Point> binMapping(numOffsets);
//
//    int numOutliers = 0;
//    for (int j = 0; j < offsets.size(); j++) {
//        Point2f offset = offsets[j];
//        int binx = (int)(offset.x / binSizeX);
//        int biny = (int)(offset.y / binSizeY);
//        if (binx < 0 || binx >= numOffsets || biny < 0 || biny >= numOffsets) {
//            numOutliers++;
//            binMapping[j] = Point(-1, -1);
//        }
//        else {
//            bins(biny, binx) = bins(biny,  binx) + 1;
//            binMapping[j] = Point(binx, biny);
//        }
//    }
//
//
//    double minVal, maxVal;
//    Point minLoc, maxLoc;
//    minMaxLoc(bins, &minVal, &maxVal, &minLoc, &maxLoc);
//
//    int numFilteredOffsets = bins(maxLoc);
//    for (int j = 0; j < numOffsets; j++) {
//        if (binMapping[j] == maxLoc) {
//            filteredOffsets.push_back(offsets[j]);
//            success[j] = true;
//        }
//    }


    return filteredOffsets;

}


