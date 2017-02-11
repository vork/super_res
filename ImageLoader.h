//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_IMAGELOADER_H
#define SUPER_RES_IMAGELOADER_H

#include <string>
#include <set>
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

/// Handles image loading
class ImageLoader {

public:

    const set<string> allowedFiletypes = {".png", ".jpg", ".jpeg", ".gif"};

    ///The filename is postfixed by _##
    void loadImages(string path, string filename, string ext, int totalImages, vector<Mat> &lowResImg);

    /**
     * Load all images in lexical order given in a directory
     * @param directory: path to the directory
     * @return vector of images
     */
    vector<Mat> loadImages(string directory);
    vector<Mat> loadImages(string directory, vector<std::string> &filenames);
};


#endif //SUPER_RES_IMAGELOADER_H
