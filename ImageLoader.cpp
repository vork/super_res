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





vector<Mat> ImageLoader::loadImages(string directory) {
    vector<string> filenames; // not used
    return loadImages(directory, filenames);
}

vector<Mat> ImageLoader::loadImages(string directory, vector<string> &filenames) {

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

            fs::path filepath = it->path();
            fs::path ext = filepath.extension();

            if (allowedFiletypes.find(ext.string()) != allowedFiletypes.end()) {

                filenames.push_back(filepath.filename().string());

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
