#include <iostream>
#include <cv.hpp>

#include "ImageLoader.h"

using namespace std;
using namespace cv;

int main() {

    ImageLoader * imageLoader = new ImageLoader();
    vector<Mat> images = imageLoader->loadImages("../projects/text/");

    for (Mat image : images) {
        imshow("default", image);
        waitKey(0);
    }

    return 0;
}