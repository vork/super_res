//
// Created by Paul Sanzenbacher on 30.11.16
//

#ifndef SUPER_RES_IMAGESET_H
#define SUPER_RES_IMAGESET_H

#include <opencv2/core/mat.hpp>

/**
 * Abstract class representing a set of images
 * can be implemented as a ring buffer or similar
 */

class ImageSet {

protected:
    unsigned int numImages;
    cv::Size imageSize;
    unsigned int currentImageIndex = 0;

public:
    unsigned int getNumImages() const {
        return numImages;
    }

    const cv::Size &getImageSize() const {
        return imageSize;
    }

    unsigned int getCurrentImageIndex() const {
        return currentImageIndex;
    }

    // destructor
    virtual ~ImageSet() { }

    // retrieve next image
    virtual cv::Mat1f next() = 0;

    // check if internal state has already been manipulated
    virtual bool isInitialState() = 0;

    // reset internal image iteration
    virtual void reset() = 0;

    // create a copy with initial state (internal counter is 0)
    virtual ImageSet * cloneAndReset() = 0;

    // create a subset of the image set with a list of indices (assume indices are ordered)
    virtual ImageSet * createSubset(std::vector<int> indices) = 0;

    /**
     * @param offsets
     * @param padding
     * @return
     */
    virtual ImageSet * computeImageSetWithOffsets(std::vector<cv::Point> offsets, int padding) = 0;

    virtual ImageSet * computeImageSetForRegion(cv::Rect region) = 0;

    // compute pixelwise median
    virtual cv::Mat1f computePixelwiseMedian() = 0;

};


#endif //SUPER_RES_IMAGESET_H
