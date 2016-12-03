#include <iostream>
#include <cv.hpp>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "OpticalFlow.h"
#include "SuperResolution.h"

using namespace std;
using namespace cv;

int main() {






    ImageLoader * imageLoader = new ImageLoader();
    vector<Mat> images = imageLoader->loadImages("../projects/text/");

    if (images.size() == 0) {
        cout << "No images found." << endl;
        exit(-1);
    }

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

    Parameters * parameters = new Parameters(images[0].size());
    SuperResolution * superResolution = new SuperResolution(parameters, imageSet);
    Mat1f hrImage = superResolution->compute();

    return 0;
}