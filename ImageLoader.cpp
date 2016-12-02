//
// Created by Benedikt Boss on 22/11/2016.
//

#include <iostream>
#include "ImageLoader.h"
#include <sstream>
#include "opencv2/highgui.hpp"
#include <opencv2/imgcodecs.hpp>


#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

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


vector<Mat> ImageLoader::loadImages(string directory) {

    vector<Mat> images;

    fs::path dirPath(directory);

    if (!fs::exists(dirPath)) {
        cout << "directory '" << directory << "' does not exist!";
    }
    if (!fs::is_directory(directory)) {
        cout << "directory '" << directory << "' is not a directory!";
    }

    fs::recursive_directory_iterator it, endit;

    for (it = fs::recursive_directory_iterator(dirPath); it != endit; it++) {
        if (fs::is_regular_file(*it)) {
            fs::path ext = it->path().extension();
            fs::path filepath = it->path();

            if (filetypes.find(ext.string()) != filetypes.end()) {

                Mat img = imread(filepath.string());
                if (img.empty()) {
                    cerr << filepath.string() << " can't be loaded!" << endl;
                    continue;
                }
                images.push_back(img);
            }


        }
    }
    return images;
}
