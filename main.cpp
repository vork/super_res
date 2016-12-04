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
    vector<Mat> images = imageLoader->loadImages("../projects/text2/");

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
    Parameters * parameters = new Parameters(images[0].size());

    // run super-resolution algorithm
    SuperResolution * superResolution = new SuperResolution(parameters, imageSet);


    /**
     * DEBUG
     */

//    Mat1f initial = readMat1fFromFile("../Matlab/SRes/init.bin", Size(91, 107));
//    Mat1f contrib = readMat1fFromFile("../Matlab/SRes/contrib.bin", Size(91, 107));
//    Mat1f hrImageD = superResolution->computeWithInitialSolutionAndSqrtContributions(initial, contrib);
//    exit(0);
    

    Mat1f hrImage = superResolution->compute();

    return 0;
}