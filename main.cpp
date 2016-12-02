#include <iostream>
#include <cv.hpp>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "OpticalFlow.h"

using namespace std;
using namespace cv;

int main() {

    ImageLoader * imageLoader = new ImageLoader();
    vector<Mat> images = imageLoader->loadImages("../projects/text/");

    // convert images to Mat1f
    vector<Mat1f> grayFloatImages;
    for (Mat image : images) {
        Mat1b grayImage;
        cvtColor(image, grayImage, CV_BGR2GRAY);
        Mat1f grayFloatImage;
        grayImage.convertTo(grayFloatImage, CV_32FC1, 1.0f / 255.0f);
        grayFloatImages.push_back(grayFloatImage);
    }

    ImageSet * imageSet = new SimpleImageSet(grayFloatImages);

    OpticalFlow * opticalFlow = new OpticalFlow();
    vector<Point2f> offsets = opticalFlow->computeOffsetsForImageSet(imageSet);

    for (Point2f offset : offsets) {
        Point roundedOffset((int)roundf(offset.x), (int)roundf(offset.y));
//        cout << roundedOffset << endl;
        cout << offset << endl;
    }

    return 0;
}