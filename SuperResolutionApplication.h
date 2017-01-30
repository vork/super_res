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

#define SCREEN_RES Eigen::Vector2i(1024, 768)
#define WINDOW_NAME "Super-Resolution"

class SuperResolutionApplication : public nanogui::Screen {

protected:
    nanogui::GLShader mShader;

    nanogui::Window *   imageWindow;

    // image view for displaying and analyzing the result
    nanogui::ImageView * resultImageView;


    cv::Mat3b currentResultImage;

    // indicates whether the optimization thread is currently running
    bool isOptimizing;

public:
    SuperResolutionApplication();
    virtual void draw(NVGcontext *ctx);
    virtual void drawContents();

    void runOptimization();



};


#endif //SUPER_RES_SUPERRESOLUTIONAPPLICATION_H
