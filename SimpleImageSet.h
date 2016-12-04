//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_SIMPLEIMAGESET_H
#define SUPER_RES_SIMPLEIMAGESET_H


#include <assert.h>
#include <iostream>
#include "ImageSet.h"


/**
 * Simplest implementation of abstract class ImageSet
 * stores all images in memory
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

        // initalize image size
        imageSize = images[0].size();
    }

    cv::Mat1f next() override {

        // get current image
        cv::Mat1f currentImage = images[currentImageIndex];

        // step image index
        currentImageIndex = (currentImageIndex + 1) % numImages;

        return currentImage;
    }

    bool isInitialState() override {

        // initial state equals to initial image index
        return currentImageIndex == 0;
    }

    void reset() override {

        //  reset image index
        currentImageIndex = 0;
    }

    ImageSet * cloneAndReset() override {

        // create a simple image set and initialize with same images
        SimpleImageSet * cloned = new SimpleImageSet(this->images);
        return cloned;
    }

    virtual ImageSet *createSubset(std::vector<int> indices) override {

        // create a subset of images
        std::vector<cv::Mat1f> subsetImages;

        // loop over all indices
        for (int index : indices) {

            // make sure that index is valid
            assert(index < numImages);

            // add image to subset
            subsetImages.push_back(images[index]);
        }

        // create new ImageSet object with subset images
        ImageSet * subset = new SimpleImageSet(subsetImages);
        return subset;
    }

    virtual cv::Mat1f computePixelwiseMedian() override;

    virtual ImageSet * computeImageSetWithOffsets(std::vector<cv::Point> offsets, int padding) override;


};


#endif //SUPER_RES_SIMPLEIMAGESET_H
