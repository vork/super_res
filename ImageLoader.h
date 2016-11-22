//
// Created by Benedikt Boss on 22/11/2016.
//

#ifndef SUPER_RES_IMAGELOADER_H
#define SUPER_RES_IMAGELOADER_H

#include <string>
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

/// Handles image loading
class ImageLoader {

    ///The filename is postfixed by _##
    void loadImages(string path, string filename, string ext, int totalImages, vector<Mat> &lowResImg);
};


#endif //SUPER_RES_IMAGELOADER_H
