//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include "ImageLoader.h"
#include "opencv2/highgui/highgui.hpp"

void ImageLoader::loadImages(string path, string filename, string ext, int totalImages, vector<Mat> &lowResImg) {
    lowResImg.resize(totalImages);
    for (int i = 0; i < totalImages; i++) {
        ostringstream stringStream;
        stringStream << path << filename << "_" << i << ext;
        Mat img = imread(stringStream.str());
        if(img.empty()) {
            cerr << stringstream.str() << " can't be loaded!" << endl;
            continue;
        }

        lowResImg.push_back(img);
    }
}
