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
#include <cv.hpp>
#include "Parameters.h"

#define SCREEN_RES Eigen::Vector2i(1024, 768)
#define WINDOW_NAME "Super-Resolution"

class SuperResolutionApplication : public nanogui::Screen {

protected:

    /* GUI variables */

    // required by nanogui for drawing window contents
    nanogui::GLShader mShader;

    // window containing resultImageView (see below)
    nanogui::Window * resultImageWindow;

    // image view for displaying and analyzing the result
    nanogui::ImageView * resultImageView;


    /* Optimization variables */

    // contains all parameters used for the optimization, do only use this parameter set!
    Parameters * optimizationParameters;


    /* Program state variables */

    // scurrent optimization is stored in this variable
    cv::Mat3b currentResultImage;

    // indicates whether the optimization thread is currently running
    bool isOptimizing;

    // set true to run the optimization in lock step, otherwise a separate thread will be used (currently not working)
    const bool runOptimizationInLockStep = true;

public:
    SuperResolutionApplication();

    // used by nanogui to update window contents
    virtual void draw(NVGcontext *ctx);
    virtual void drawContents();

    // start optimization
    void runOptimization(uint maxIterations, int p, uint padding, float alpha, float beta, float lambda, uint resolutionFactor);
};


#endif //SUPER_RES_SUPERRESOLUTIONAPPLICATION_H
