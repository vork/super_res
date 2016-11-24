//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include "ImageLoader.h"
#include <sstream>
#include "opencv2/highgui.hpp"
#include <opencv2/imgcodecs.hpp>

using namespace std;

void ImageLoader::loadImages(string path, string filename, string ext, int totalImages, vector<Mat> &lowResImg) {
    lowResImg.resize(totalImages);
    for (int i = 0; i < totalImages; i++) {
        ostringstream stringStream;
        stringStream << path << filename << "_" << i << ext;
        Mat img = imread(stringStream.str());
        if(img.empty()) {
            cerr << stringStream.str() << " can't be loaded!" << endl;
            continue;
        }

        lowResImg.push_back(img);
    }
}
