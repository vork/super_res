//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_SIMPLEIMAGESET_H
#define SUPER_RES_SIMPLEIMAGESET_H


#include <assert.h>
#include <iostream>
#include "ImageSet.h"

/**
 * Simplest implementation of the interface ImageSet
 */

class SimpleImageSet : public ImageSet {

protected:

    std::vector<cv::Mat1f> images;

public:

    SimpleImageSet(std::vector<cv::Mat1f> images) : images(images) {

        // make sure there is at least one image
        assert(images.size() > 0);

        // initalize num images
        numImages = images.size();
    }

    cv::Mat1f next() {

        // get current image
        cv::Mat1f currentImage = images[currentImageIndex];

        // step image index
        currentImageIndex = (currentImageIndex + 1) % numImages;

        return currentImage;
    }

    bool isInitialState() {

        // initial state equals to initial image index
        return currentImageIndex == 0;
    }

    void reset() {

        //  reset image index
        currentImageIndex = 0;
    }

    ImageSet * cloneAndReset() {

        // create a simple image and initialize with same images
        SimpleImageSet * cloned = new SimpleImageSet(this->images);
        return cloned;
    }

};


#endif //SUPER_RES_SIMPLEIMAGESET_H
