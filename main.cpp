#include <iostream>
#include <cv.hpp>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "OpticalFlow.h"
#include "SuperResolution.h"
#include "util.h"

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
        grayImage.convertTo(grayFloatImage, CV_32FC1);
        grayFloatImages.push_back(grayFloatImage);
    }

    // create simple image set (all images are stores in RAM)
    ImageSet * imageSet = new SimpleImageSet(grayFloatImages);

    // create default parameter set
    Parameters * parameters = new Parameters(imageSet);

    // run super-resolution algorithm
    SuperResolution * superResolution = new SuperResolution(parameters);
    Mat1f hrImage = superResolution->compute();

    // write result
    imwrite("hr.png", hrImage);

    return 0;
}