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
    Window * mainWindow = new Window(this, "Test Window");
    mainWindow->setLayout(new GroupLayout());

    // Create optimization Button and set callback
    Button * optimizeButton = new Button(mainWindow, "Optimize");
    optimizeButton->setCallback([this] {

        // run optimization in separate thread
//        if (!isOptimizing) {
//            isOptimizing = true;
//            thread optimizationThread([this] {
//                this->runOptimization();
//            });
//            optimizationThread.detach();
//        }

        // run optimization in lock step
        this->runOptimization();

    });

    string placeholderFilename = "resources/result_placeholder.png";
    currentResultImage = imread(placeholderFilename);
    if (!currentResultImage.data) {
        cout << "image '" << placeholderFilename << "' can not be found." << endl;
        exit(0);
    }

    Texture * placeholderTexture = new Texture(currentResultImage, "result placeholder");

    imageWindow = new Window(this, "Result");
    imageWindow->setPosition(Vector2i(100, 15));
    imageWindow->setLayout(new GroupLayout());

    resultImageView = new ImageView(imageWindow, placeholderTexture->getTextureId());

    performLayout();

    this->runOptimization();
}

void SuperResolutionApplication::runOptimization() {

    ImageLoader * imageLoader = new ImageLoader();
    vector<Mat> images = imageLoader->loadImages("images/");

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

    // create simple image set (all images are stores in RAM)
    ImageSet * imageSet = new SimpleImageSet(grayFloatImages);

    // create default parameter set
    Parameters * parameters = new Parameters(imageSet);

    // run super-resolution algorithm
    SuperResolution * superResolution = new SuperResolution(parameters);
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

