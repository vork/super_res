//
// Created by Paul Sanzenbacher on 29.01.17
//

#ifndef SUPER_RES_SUPERRESOLUTIONAPPLICATION_H
#define SUPER_RES_SUPERRESOLUTIONAPPLICATION_H

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <string>
#include <nanogui/imageview.h>
#include <nanogui/common.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <cv.hpp>
#include "Parameters.h"
#include <GLFW/glfw3.h>


#define WINDOW_NAME "Super-Resolution"

class SuperResolutionApplication : public nanogui::Screen {

protected:

    /* GUI variables */

    Eigen::Vector2i windowSize;

    // label displaying the current directory path;
    nanogui::Label * directoryLabel;

    // required by nanogui for drawing window contents
    nanogui::GLShader mShader;

    // scroll panel and image panel for the source images
    nanogui::VScrollPanel * scrollPanel;
    nanogui::ImagePanel * imagePanel;

    // window containing resultImageView (see below)
    nanogui::Window * resultImageWindow;

    // image view for displaying and analyzing the result
    nanogui::ImageView * resultImageView;

    /* Optimization variables */

    // contains all parameters used for the optimization, do only use this parameter set!
    Parameters * optimizationParameters;


    /* Program state variables */

    // low resolution images
    std::vector<cv::Mat> sourceImages;

    // current directory path
    std::string directoryPath;

    // scurrent optimization is stored in this variable
    cv::Mat3b currentResultImage;

    // indicates whether the optimization thread is currently running
    bool isOptimizing;

    // determines whether the result image view will be updated after each iteration
    bool showIterationResults = false;

    // determines wheater the result image view will be in color or not
    bool hiResColor = false;

    // set true to run the optimization in lock step, otherwise a separate thread will be used (currently not working)
    const bool runOptimizationInLockStep = true;

public:
    SuperResolutionApplication(Eigen::Vector2i _windowSize);

    // used by nanogui to update window contents
    virtual void draw(NVGcontext *ctx);
    virtual void drawContents();

    // start optimization
    void runOptimization(uint maxIterations, int p, uint padding, float alpha, float beta, float lambda,
                         uint resolutionFactor);

    // load images from current directory
    void loadImages();

    void displayImage(cv::Mat3b image) const;
};


#endif //SUPER_RES_SUPERRESOLUTIONAPPLICATION_H
