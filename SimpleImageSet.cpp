//
// Created by Paul Sanzenbacher on 30.11.16
//

#include "SimpleImageSet.h"
#include "util.h"

using namespace std;
using namespace cv;

ImageSet * SimpleImageSet::computeImageSetWithOffsets(vector<Point> offsets, int padding) {
    assert(numImages == offsets.size());

    vector<Mat1f> offsetImages;

    for (int i = 0; i < numImages; i++) {
        Mat1f offsetImage = shiftImage(images[i], offsets[i], padding);
        offsetImages.push_back(offsetImage);
    }

    ImageSet * offsetImageSet = new SimpleImageSet(offsetImages);
    return offsetImageSet;
}

Mat1f SimpleImageSet::computePixelwiseMedian() {

    float buffer[numImages];

    Size size = images[0].size();

    Mat1f medianImage(size);
    float * medianImageData = (float *)medianImage.data;

    // create data pointers for all images
    float * data[numImages];
    for (int i = 0; i < numImages; i++) {
        data[i] = (float *)images[i].data;
    }

    // iterate over all pixels
    unsigned int numPixles = size.width * size.height;

    for (int i = 0; i < numPixles; i++) {

        // fill buffer
        for (int j = 0; j < numImages; j++) {
            buffer[j] = data[j][i];
        }

        // compute median
        float median = medianOfFloatArray(buffer, numImages);

        // write median to median image
        medianImageData[i] = median;
    }

    return medianImage;
}
