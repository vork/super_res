//
// Created by Paul Sanzenbacher on 29.01.17
//

#include "SuperResolutionApplication.h"

#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <thread>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "OpticalFlow.h"
#include "SuperResolution.h"
#include "Texture.h"



using namespace nanogui;
using namespace std;
using namespace cv;


SuperResolutionApplication::SuperResolutionApplication() : nanogui::Screen(SCREEN_RES, WINDOW_NAME) {

    isOptimizing = false;

    // Create main window
    Window * controlsWindow = new Window(this, "Controls");
    controlsWindow->setLayout(new GroupLayout());

    // Create optimization Button and set callback
    Button * optimizeButton = new Button(controlsWindow, "Optimize");

    // Start optimization on press button
    optimizeButton->setCallback([this] {

        if (runOptimizationInLockStep) {
            this->runOptimization();
        }
        else {
            // run optimization in separate thread
            // fixme: currently not working
            if (!isOptimizing) {
                isOptimizing = true;
                thread optimizationThread([this] {
                    this->runOptimization();
                });
                optimizationThread.detach();
            }
        }
    });

    // Initialize image view with a placeholder image
    string placeholderFilename = "resources/result_placeholder.png";
    currentResultImage = imread(placeholderFilename);
    if (!currentResultImage.data) {
        cout << "image '" << placeholderFilename << "' can not be found." << endl;
        exit(0);
    }

    // Convert OpenCV image to OpenGL texture wrapper object
    Texture * placeholderTexture = new Texture(currentResultImage, "result placeholder");

    // Create image window and view and display placeholder image
    resultImageWindow = new Window(this, "Result");
    resultImageWindow->setPosition(Vector2i(100, 15));
    resultImageWindow->setLayout(new GroupLayout());
    resultImageView = new ImageView(resultImageWindow, placeholderTexture->getTextureId());

    // required by nanogui to render GUI
    performLayout();
}

void SuperResolutionApplication::runOptimization() {

    // TODO: set image directory from user selection in GUI
    string imageDirectoryPath = "images/";

    ImageLoader * imageLoader = new ImageLoader();
    vector<Mat> images = imageLoader->loadImages(imageDirectoryPath);

    if (images.size() == 0) {
        cout << "No images found." << endl;
    }

    // convert images to Mat1f
    vector<Mat1f> grayFloatImages;
    for (Mat image : images) {
        Mat1b grayImage;
        cvtColor(image, grayImage, CV_BGR2GRAY);
        Mat1f grayFloatImage;
        grayImage.convertTo(grayFloatImage, CV_32FC1);
        grayFloatImages.push_back(grayFloatImage);
    }

    // create simple image set (all images are stored in RAM)
    ImageSet * imageSet = new SimpleImageSet(grayFloatImages);

    // create default parameter set
    optimizationParameters = new Parameters(imageSet);

    // run super-resolution algorithm
    SuperResolution * superResolution = new SuperResolution(optimizationParameters);
    Mat1f hrImage = superResolution->compute();


    // convert result to displayable format
    Mat1b bResult;
    hrImage.convertTo(bResult, CV_8UC1);
    cvtColor(bResult, currentResultImage, CV_GRAY2BGR);

    Texture * texture = new Texture(currentResultImage, "result");

    resultImageView->bindImage(texture->getTextureId());

    isOptimizing = false;
}

void SuperResolutionApplication::draw(NVGcontext *ctx) {

    /* Draw the user interface */
    Screen::draw(ctx);
}

void SuperResolutionApplication::drawContents() {

//    mShader.bind();
}

