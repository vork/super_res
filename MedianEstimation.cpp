//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include <map>
#include "MedianEstimation.h"


using namespace cv;
using namespace std;


MedianEstimation::MedianEstimation(ImageSet *imageSet, Parameters *parameters, const vector<Point2f> &offsets)
        : imageSet(imageSet), parameters(parameters), offsets(offsets) {}

cv::Mat1f MedianEstimation::computeEstimatedSuperResolution(Mat1f &sqrtContributions) {

    Size lrSize = parameters->getLowResSize();
    Size hrSize = parameters->calcHighResSize();
    int resolutionFactor = parameters->getResolutionFactor();

    // compute discrete offsets by rounding the offsets and scale them to the high resolution domain
    vector<Point> discreteOffsets;
    for (Point2f p : offsets) {

        Point dp((int)roundf(p.x * resolutionFactor), (int)roundf(p.y * resolutionFactor));
        discreteOffsets.push_back(dp);
    }

    // estimated high resolution image
    Mat1f estimate(hrSize, 0.0f);
    sqrtContributions = Mat1f(hrSize, 0.0f);

    // map that indicates if a value has been found for each pixel using the median method
//    map<Point, bool> foundImageForOffsets;

    // iterate over each possible displacement value
    for (int xOffset = 1 - resolutionFactor; xOffset < resolutionFactor; xOffset++) {
        for (int yOffset = 1 - resolutionFactor; yOffset < resolutionFactor; yOffset++) {

//            foundImageForOffsets[Point(xOffset, yOffset)] = false;

            // calculate how often this offset is contained in the offsets vector and store corresponding image indices
            vector<int> currentOffsetIndices;
            for (int i = 0; i < discreteOffsets.size(); i++) {
                Point offset = discreteOffsets[i];
                if (offset.x == xOffset && offset.y == yOffset) {
                    currentOffsetIndices.push_back(i);
                }
            }

            cout << "Images for offset " << Point(xOffset, yOffset) << ": " << currentOffsetIndices.size() << endl;

            // only compute median if there exist images for this offset
            if (currentOffsetIndices.size() > 0) {

                float sqrtContribution = sqrtf(currentOffsetIndices.size());

//                foundImageForOffsets[Point(xOffset, yOffset)] = true;

                // get subset of images with current offset
                ImageSet * imageSubset = imageSet->createSubset(currentOffsetIndices);

                // compute pixel wise median
                Mat1f medianSubimage = imageSubset->computePixelwiseMedian();

                // insert median sub image to estimated high resolution image
                for (int y = 0; y < medianSubimage.rows; y++) {
                    float * medianSubimageRowPtr = medianSubimage.ptr<float>(y);

                    int estimatedY = y * resolutionFactor + yOffset;
                    if (estimatedY < 0) {
                        continue;
                    }
                    if (estimatedY >= hrSize.height) {
                        break;
                    }

                    float * estimatedRowPtr = estimate.ptr<float>(estimatedY);
                    float * sqrtContributionsRowPtr = sqrtContributions.ptr<float>(estimatedY);

                    for (int x = 0; x < medianSubimage.cols; x++) {
                        int estimatedX = x * resolutionFactor + xOffset;
                        if (estimatedX < 0 || estimatedX >= hrSize.width) {
                            continue;
                        }

                        estimatedRowPtr[estimatedX] = medianSubimageRowPtr[x];
                        sqrtContributionsRowPtr[estimatedX] = sqrtContribution;
                    }
                }

                // delete image subset again
                delete imageSubset;
            }
        }
    }

    // TODO: fill black holes

    return estimate;
}

const Mat1f &MedianEstimation::getSqrtContributions() const {
    return sqrtContributions;
}
