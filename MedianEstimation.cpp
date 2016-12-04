//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include <map>
#include "MedianEstimation.h"
#include "util.h"


using namespace cv;
using namespace std;


MedianEstimation::MedianEstimation(Parameters *parameters, const vector<Point2f> &offsets):
        parameters(parameters), offsets(offsets) {}

cv::Mat1f MedianEstimation::computeEstimatedSuperResolution(Mat1f &sqrtContributions) {

    const int padding = parameters->getPadding(); // crop to reduce black borders

    int resolutionFactor = parameters->getResolutionFactor();


    // offset of each image in low resolution space
    vector<Point> imageOffsets;

    // discrete offsets for each image in high resolution space (relative to offset in low resolution space)
    vector<Point> subpixelOffsets;

    for (Point2f p : offsets) {

        Point d((int)roundf(p.x * resolutionFactor), (int)roundf(p.y * resolutionFactor));

        Point discreteScaled;
        discreteScaled.x = mod(d.x, resolutionFactor);
        discreteScaled.y = mod(d.y, resolutionFactor);
        Point imageOffset = Point((int)floorf((float)d.x / resolutionFactor), (int)floorf((float)d.y / resolutionFactor));

        imageOffsets.push_back(imageOffset);
        subpixelOffsets.push_back(discreteScaled);
    }

    // align all images in discrete space
    ImageSet * croppedShiftedImageSet = parameters->getImageSet()->computeImageSetWithOffsets(imageOffsets, padding);
    parameters->setImageSet(croppedShiftedImageSet);

    // estimated high resolution image
    Mat1f estimate(parameters->getHighResSize(), 0.0f);
    sqrtContributions = Mat1f(parameters->getHighResSize(), 1.0f);

    // iterate over each possible displacement value
    for (int xOffset = 0; xOffset < resolutionFactor; xOffset++) {
        for (int yOffset = 0; yOffset < resolutionFactor; yOffset++) {


            // calculate how often this offset is contained in the offsets vector and store corresponding image indices
            vector<int> currentOffsetIndices;
            for (int i = 0; i < subpixelOffsets.size(); i++) {
                Point offset = subpixelOffsets[i];
                if (offset.x == xOffset && offset.y == yOffset) {
                    currentOffsetIndices.push_back(i);
                }
            }

//            cout << "Images for offset " << Point(xOffset, yOffset) << ": " << currentOffsetIndices.size() << endl;

            // only compute median if there exist images for this offset
            if (currentOffsetIndices.size() > 0) {

                float sqrtContribution = sqrtf(currentOffsetIndices.size());

                // get subset of images with current offset
                ImageSet * imageSubset = parameters->getImageSet()->createSubset(currentOffsetIndices);

                // compute pixel wise median
                Mat1f medianSubimage = imageSubset->computePixelwiseMedian();

                Size lrSize = parameters->getLowResSize();

                // insert median sub image to estimated high resolution image
                for (int y = 0; y < lrSize.height; y++) {

                    int hry = y * resolutionFactor + yOffset; // corresponding high resolution y

                    float * medianSubimageRowPtr = medianSubimage.ptr<float>(y);
                    float * estimatedRowPtr = estimate.ptr<float>(hry + 1);
                    float * sqrtContributionsRowPtr = sqrtContributions.ptr<float>(hry + 1);

                    for (int x = 0; x < lrSize.width; x++) {

                        int hrx = x * resolutionFactor + xOffset;

                        estimatedRowPtr[hrx + 1] = medianSubimageRowPtr[x];
                        sqrtContributionsRowPtr[hrx + 1] = sqrtContribution;
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
