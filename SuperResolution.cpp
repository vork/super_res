//
// Created by Paul Sanzenbacher on 30.11.16
//

#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>
#include "SuperResolution.h"
#include "OpticalFlow.h"
#include "util.h"
#include "SimpleImageSet.h"

using namespace cv;
using namespace std;

SuperResolution::SuperResolution(Parameters *parameters, bool _computeColoredResult) {
    computeColoredResult = _computeColoredResult;
    this->parameters = parameters;
}

cv::Mat1f SuperResolution::gradientBackProject() {

    // back projected matrix
    Mat1f back;

    // store point spread function
    PointSpreadFunction * psf = parameters->getPointSpreadFunction();

    // blur current solution using the point spread function
    const int ddepth = -1; // use same depth for dst image
    const Point anchorPoint = Point(-1, -1); // use center as anchor point
    const double delta = 0.0; // OpenCV default
    const int borderType = BORDER_REFLECT;
    Mat1f kernel = psf->getKernel();
    filter2D(hrImage, back, ddepth, kernel, anchorPoint, delta, borderType);

    // compute difference between blurred current solution and initial solution
     back = back - initialSolution;

    // weight with sqrtContributions
     back = sqrtContributions.mul(back);

    // compute sign for each value
    int numPixels = parameters->getHighResSize().width * parameters->getHighResSize().height;
    float * backData = (float *)back.data;
    for (int i = 0; i < numPixels; i++) {
        backData[i] = signf(backData[i]);
    }

    // unblur backprojected image
    PointSpreadFunction * flippedPSF = psf->flipped();
    kernel = flippedPSF->getKernel();

    Mat1f unfiltered;
    back = sqrtContributions.mul(back);
    filter2D(back, unfiltered, ddepth, kernel, anchorPoint, delta, borderType);
    delete flippedPSF;

    return unfiltered;
}

cv::Mat1f SuperResolution::gradientRegulization() {

    // get window radius
    const int p = parameters->getP();

    // get constant alpha from parameter set
    const float alpha = parameters->getAlpha();

    // regulized matrix
    Mat1f reg(parameters->getHighResSize(), 0.0f);

    // create mirror padded version of current solution
    Mat1f padded;
    copyMakeBorder(hrImage, padded, p, p, p, p, BORDER_REFLECT);

    Mat1f shifted;


    printf("p: %i\n", p);

    // iterate over window
    for (int l = -p; l <= p; l++) {
        for (int m = -p; m <= p; m++) {

            Rect roi(Point(p - l, p - m), parameters->getHighResSize());

            Mat1f shift = padded(roi);

            Mat1f diff = hrImage - shift;

            // compute sign
            Mat1f sign(parameters->getHighResSize());
            const int numPixels = parameters->getHighResSize().width * parameters->getHighResSize().height;
            float * signData = (float *)sign.data;
            float * diffData = (float *)diff.data;
            for (int i = 0; i < numPixels; i++) {
                signData[i] = (float)sgn<float>(diffData[i]);
            }

            Mat1f signPadded;
            copyMakeBorder(sign, signPadded, p, p, p, p, BORDER_CONSTANT, 0.0f);

            shift = signPadded(Rect(Point(p + l, p + m), parameters->getHighResSize()));

            diff = sign - shift;

            const float factor = powern(alpha, abs(l) + abs(m));

            reg = reg + factor * diff;
        }
    }
    return reg;
}

Mat3f SuperResolution::compute() {

    timer.reset();


    Timer completeTimer;
    completeTimer.reset();

    vector<Mat1f> grayImages;
    vector<Mat1f> crImages, cbImages;
    for (Mat3f rgbImage : parameters->getLowResolutionImages()) {
        Mat3f yCrCbImage = convertToYCrCb(rgbImage);
        vector<Mat1f> channels(3);
        split(rgbImage, channels);
        grayImages.push_back(channels[0]);
        crImages.push_back(channels[1]);
        cbImages.push_back(channels[2]);
    }


    unique_ptr<OpticalFlow>opticalFlow(new OpticalFlow());
    ImageSet * grayImageSet = new SimpleImageSet(grayImages);
    vector<Point2f> imageFlows = opticalFlow->computeOffsetsForImageSet(grayImageSet);

    // compute offsets as negative image flow
    vector<Point2f> offsets;
    for (Point2f imageFlow : imageFlows) {
        offsets.push_back((-1) * imageFlow);
    }

    timer.printTimeAndReset("offset computation");

    // compute median estimation
    MedianEstimationResult medianEstimationResult = medianEstimation(grayImageSet, offsets);

    Mat3f hrGray = medianEstimationResult.medianEstimate;
    sqrtContributions = medianEstimationResult.sqrtContributions;

    timer.printTimeAndReset("median estimation");

    // DEBUG: write median estimation image
    imwrite("median.png", hrImage);

    hrGray = computeWithInitialSolutionAndSqrtContributions(hrImage, sqrtContributions);

    if (computeColoredResult) {
        unique_ptr<ImageSet> crImageSet(new SimpleImageSet(crImages));
        MedianEstimationResult crMEResult = medianEstimation(crImageSet.get(), offsets);
        Mat1f crResult = crMEResult.medianEstimate;
        unique_ptr<ImageSet> cbImageSet(new SimpleImageSet(cbImages));
        MedianEstimationResult cbMEResult = medianEstimation(cbImageSet.get(), offsets);
        Mat1f cbResult = cbMEResult.medianEstimate;
        vector<Mat1f> channels(3);
        channels[0] = hrGray;
        channels[1] = crResult;
        channels[2] = cbResult;
        Mat3f yCrCbResult;
        merge(channels, yCrCbResult);
        cvtColor(yCrCbResult, hrImage, CV_YCrCb2RGB);

        // TODO: test
    }
    else {
        // distribute gray information to all channels
        cvtColor(hrGray, hrImage, CV_GRAY2RGB);
    }

    completeTimer.printTimeAndReset("complete optimization time");

    return hrImage;
}

Mat1f SuperResolution::computeWithInitialSolutionAndSqrtContributions(Mat1f _initialSolution, Mat1f _sqrtContributions) {

    timer.reset();

    initialSolution = _initialSolution.clone();
    Mat1f hrGray = _initialSolution;
    sqrtContributions = _sqrtContributions;


    const float beta = parameters->getBeta();
    const float lambda = parameters->getLambda();
    Mat1f gradient(parameters->getHighResSize());

    Timer iterationTimer;

    for (unsigned int iteration = 0; iteration < parameters->getMaxIterations(); iteration++) {

        // gradient back projection
        Mat1f backProjected = gradientBackProject();
        iterationTimer.printTimeAndReset("gradient back project");

        Mat1f regularized = gradientRegulization();
        iterationTimer.printTimeAndReset("regulization");

        // compute gradient
         gradient = backProjected + (lambda * regularized);

        // subtract gradient
        hrGray = hrGray - (beta * gradient);

        // call iteration callback with intermediate result
        if (iterationCallback) {
            iterationCallback(hrGray);
        }

        iterationTimer.printTimeAndReset("rest");

        timer.setMarker();
        cout << "iteration " << iteration << " done." << endl;
    }

    timer.printTimeAndReset("gradient descent");

    return hrGray;
}

void SuperResolution::setIterationCallback(const function<void(Mat1f)> &iterationCallback) {
    SuperResolution::iterationCallback = iterationCallback;
}

//cv::Mat SuperResolution::extractColorInformation() {
//    //Get the reference
//    Mat referencef = parameters->getRefImage();
//
//    //Resize the lowres ref image to the
//    Mat refResize;
//    Size size(hrImage.cols, hrImage.rows);
//    resize(referencef, refResize, size); //TODO maybe we need to set a better interpolation?
//
//    //Convert the images to lab
//    Mat refLab;
//    cvtColor(refResize, refLab, CV_BGR2Lab);
//
//    //Convert the HiRes image to BGR
//    Mat hr = hrImage;
//
//    refLab.convertTo(refLab, CV_8UC3); //LabHi Should be the same depth as labRef.
//    hr.convertTo(hr, CV_8UC1);
//
//    //now the channels are in LAB (X, Y, Z).
//    //So take the Y and Z Channel from the upscaled color image and insert them
//    //in the hires image
//    vector<Mat> refChannels(3);
//    split(refLab, refChannels);
//
//    vector<Mat> retChannels;
//    retChannels.push_back(hr);
//    retChannels.push_back(refChannels[1]);
//    retChannels.push_back(refChannels[2]);
//
//    Mat retLab;
//    merge(retChannels, retLab);
//
//    //Convert back to BGR
//    Mat ret;
//    cvtColor(retLab, ret, CV_Lab2BGR);
//
//    return ret;
//}

MedianEstimationResult SuperResolution::medianEstimation(ImageSet * imageSet, PointList offsets) {

    // IMPORTANT: use images only from local imageSet, not from parameters object

    const int padding = parameters->getPadding(); // crop to reduce black borders
    const int resolutionFactor = parameters->getResolutionFactor();

    // offset of each image in low resolution space
    vector<Point> imageOffsets;

    // discrete offsets for each image in high resolution space (relative to offset in low resolution space)
    vector<Point> subpixelOffsets;

    for (Point2f p : offsets) {

        Point d((int) roundf(p.x * resolutionFactor), (int) roundf(p.y * resolutionFactor));

        Point discreteScaled;
        discreteScaled.x = mod(d.x, resolutionFactor);
        discreteScaled.y = mod(d.y, resolutionFactor);
        Point imageOffset = Point((int) floorf((float) d.x / resolutionFactor),
                                  (int) floorf((float) d.y / resolutionFactor));

        imageOffsets.push_back(imageOffset);
        subpixelOffsets.push_back(discreteScaled);
    }

    // align all images in discrete space
    ImageSet *croppedShiftedImageSet = imageSet->computeImageSetWithOffsets(imageOffsets, padding);

    // estimated high resolution image
    Mat1f estimate(parameters->getHighResSize(), 0.0f);
    sqrtContributions = Mat1f(parameters->getHighResSize(), 1.0f);

    // save if there is a contribution for each offset to identify black spots in next step
    bool hasContributionForOffset[resolutionFactor][resolutionFactor];
    int sumContributedOffsets = 0;

    // iterate over each possible displacement value
    for (int yOffset = 0; yOffset < resolutionFactor; yOffset++) {
        for (int xOffset = 0; xOffset < resolutionFactor; xOffset++) {

            // initialize hasContribuationForOffset with false, set to true when there is a contribution
            hasContributionForOffset[yOffset][xOffset] = false;

            // calculate how often this offset is contained in the offsets vector and store corresponding image indices
            vector<int> currentOffsetIndices;
            for (int i = 0; i < subpixelOffsets.size(); i++) {
                Point offset = subpixelOffsets[i];
                if (offset.x == xOffset && offset.y == yOffset) {
                    currentOffsetIndices.push_back(i);
                }
            }

            // only compute median if there exist images for this offset
            if (currentOffsetIndices.size() > 0) {

                // there is at least one contribution
                hasContributionForOffset[yOffset][xOffset] = true;
                sumContributedOffsets += 1;

                float sqrtContribution = sqrtf(currentOffsetIndices.size());

                // get subset of images with current offset
                ImageSet *imageSubset = croppedShiftedImageSet->createSubset(currentOffsetIndices);

                // compute pixel wise median
                Mat1f medianSubimage = imageSubset->computePixelwiseMedian();

                Size lrSize = parameters->getLowResSize();

                // insert median sub image to estimated high resolution image
                for (int y = 0; y < lrSize.height; y++) {

                    int hry = y * resolutionFactor + yOffset; // corresponding high resolution y

                    float *medianSubimageRowPtr = medianSubimage.ptr<float>(y);
                    float *estimatedRowPtr = estimate.ptr<float>(hry + 1);
                    float *sqrtContributionsRowPtr = sqrtContributions.ptr<float>(hry + 1);

                    for (int x = 0; x < lrSize.width; x++) {

                        int hrx = x * resolutionFactor + xOffset;

                        estimatedRowPtr[hrx + 1] = medianSubimageRowPtr[x];
                        sqrtContributionsRowPtr[hrx + 1] = sqrtContribution;
                    }
                }

                // delete image subset again
                delete imageSubset;
            }
        }
    }


    float contributionRate = (float)sumContributedOffsets / (float)(resolutionFactor * resolutionFactor);

    if (contributionRate <= 0.5f) {
        cout << "Warning: a contribution rate of over 50% is necessary to compute good results." << endl;
        cout << "Current contribution rate is " << contributionRate << "." << endl;
        cout << "Maybe you need to reduce the resolution factor." << endl << endl;
    }

    // fill black spots with median filter
    for (int yOffset = 0; yOffset < resolutionFactor; yOffset++) {
        for (int xOffset = 0; xOffset < resolutionFactor; xOffset++) {

            if (!hasContributionForOffset[yOffset][xOffset]) {

                // iterate over all black spots
                for (int y = yOffset; y < estimate.rows - 1; y += resolutionFactor) {
                    for (int x = xOffset; x < estimate.cols -1; x += resolutionFactor) {

                        int fy = y / resolutionFactor;
                        int fx = x / resolutionFactor;

                        float buffer[sumContributedOffsets];

                        int i = 0;
                        for (int iy = 0; iy < resolutionFactor; iy++) {
                            for (int ix = 0; ix < resolutionFactor; ix++) {
                                if (hasContributionForOffset[iy][ix]) {
                                    buffer[i] = estimate(fy * resolutionFactor + iy + 1, fx * resolutionFactor + ix + 1);
                                    i++;
                                }
                            }
                        }

                        float median = medianOfFloatArray(buffer, sumContributedOffsets);

                        estimate(y + 1, x + 1) = median;
                    }
                }
            }
        }
    }

    MedianEstimationResult result(
        estimate,
        sqrtContributions
    );

    return result;
}

