//
// Created by Paul Sanzenbacher on 29.01.17
//

#include "SuperResolutionApplication.h"

#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/imageview.h>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "OpticalFlow.h"
#include "util.h"
#include "SuperResolution.h"
#include <thread>



using namespace nanogui;
using namespace std;
using namespace cv;


// TODO: not working yet
GLuint matToTexture(Mat texture_cv) {

    glEnable(GL_TEXTURE_2D);

    GLuint texture;

// Create Texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // 2d texture (x and y size)

//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture


// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_cv.cols, texture_cv.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_cv.data);

    glBindTexture(GL_TEXTURE_2D, texture); // choose the texture to use.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texture;
}


SuperResolutionApplication::SuperResolutionApplication() : nanogui::Screen(SCREEN_RES, WINDOW_NAME) {


    isOptimizing = false;

    // Create main window
    Window * mainWindow = new Window(this, "Test Window");
    mainWindow->setLayout(new GroupLayout());

    // Create optimization Button and set callback
    Button * optimizeButton = new Button(mainWindow, "Optimize");
    optimizeButton->setCallback([this] {

        // run optimization in separate thread
        if (!isOptimizing) {
            isOptimizing = true;
            thread optimizationThread([this] {
                this->runOptimization();
            });
            optimizationThread.detach();
        }
    });


//    Mat image = imread("median.png");
//    if (!image.data) {
//        cout << "no data" << endl;
//    }
//
//    auto imageWindow = new Window(this, "Result");
//    imageWindow->setPosition(Vector2i(100, 15));
//    imageWindow->setLayout(new GroupLayout());
//
//    auto imageView = new ImageView(imageWindow, matToTexture(image));

    performLayout();
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

    // write result
//    imwrite("hr.png", hrImage);

    isOptimizing = false;

}

void SuperResolutionApplication::draw(NVGcontext *ctx) {

    /* Draw the user interface */
    Screen::draw(ctx);
}

void SuperResolutionApplication::drawContents() {


    /* Draw the window contents using OpenGL */
    mShader.bind();
}

