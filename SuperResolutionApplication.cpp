//
// Created by Paul Sanzenbacher on 29.01.17
//

#include "SuperResolutionApplication.h"
//
//#include <nanogui/layout.h>
//#include <nanogui/button.h>
//#include <nanogui/label.h>
//#include <nanogui/combobox.h>
//TODO include nur benötigte
#include <nanogui/nanogui.h>


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

    // ------ Create Low Resolution Image Panel ----------
    Window * lowResImgs = new Window(this, "Low Resolution Images");
    lowResImgs->setPosition(Vector2i(20, 20));
    lowResImgs->setFixedSize(Vector2i(450, 300));
    lowResImgs->setLayout(new GroupLayout());

    // ------ Create Control Parameter Window ----------
    Window * controlParams = new Window(this, "Control Parameters");

    //Layout: Window for Control Parameters
    controlParams->setPosition(Vector2i(20, 350));
    controlParams->setFixedSize(Vector2i(450, 400));

    GridLayout *layout =
            new GridLayout(Orientation::Horizontal, 2,
                           Alignment::Middle, 15, 5);
    layout->setColAlignment(
            { Alignment::Maximum, Alignment::Fill });
    layout->setSpacing(0, 10);
    controlParams->setLayout(layout);


    /*Resolution Factor Label and Box */
    new Label(controlParams, "resolution factor:", "sans");
    auto intBoxResolution = new IntBox<int>(controlParams);
    intBoxResolution->setEditable(true);
    intBoxResolution->setFixedSize(Vector2i(80, 20));
    intBoxResolution->setValue(2);
    intBoxResolution->setUnits("int");
    intBoxResolution->setDefaultValue("2");
    intBoxResolution->setFontSize(16);
    intBoxResolution->setFormat("[1-9][0-9]*");
    intBoxResolution->setSpinnable(true);
    intBoxResolution->setMinValue(1);
    intBoxResolution->setValueIncrement(1);
    //TODO: Max Value setzen?


    /* Alpha Label and Box */
    new Label(controlParams, "alpha:", "sans");
    auto floatBoxAlpha = new FloatBox<float>(controlParams);
    floatBoxAlpha->setEditable(true);
    floatBoxAlpha->setFixedSize(Vector2i(120, 20));
    floatBoxAlpha->setValue(0.7f);
    floatBoxAlpha->setUnits("float");
    floatBoxAlpha->setDefaultValue("0.7");
    floatBoxAlpha->setFontSize(16);
    floatBoxAlpha->setFormat("[0-9].[0-9][0-9]*");
    floatBoxAlpha->setSpinnable(true);
    floatBoxAlpha->setMinValue(0.0f);
    floatBoxAlpha->setValueIncrement(0.1f);
    //TODO gibt es einen Max Value?



    /* Beta Label and Box */
    new Label(controlParams, "beta:", "sans");
    auto floatBoxBeta = new FloatBox<float>(controlParams);
    floatBoxBeta->setEditable(true);
    floatBoxBeta->setFixedSize(Vector2i(120, 20));
    floatBoxBeta->setValue(1.0f);
    floatBoxBeta->setUnits("float");
    floatBoxBeta->setDefaultValue("1.0");
    floatBoxBeta->setFontSize(16);
    floatBoxBeta->setFormat("[0-9].[0-9][0-9]*");
    floatBoxBeta->setSpinnable(true);
    floatBoxBeta->setMinValue(0.0f);
    floatBoxBeta->setValueIncrement(0.1f);
    //TODO gibt es einen Max Value?


    /* Lambda Label and Box */
    new Label(controlParams, "lambda:", "sans");
    auto floatBoxLambda = new FloatBox<float>(controlParams);
    floatBoxLambda->setEditable(true);
    floatBoxLambda->setFixedSize(Vector2i(120, 20));
    floatBoxLambda->setValue(0.04f);
    floatBoxLambda->setUnits("float");
    floatBoxLambda->setDefaultValue("0.04");
    floatBoxLambda->setFontSize(16);
    floatBoxLambda->setFormat("[0-9].[0-9][0-9]*");
    floatBoxLambda->setSpinnable(true);
    floatBoxLambda->setMinValue(0.0f);
    floatBoxLambda->setValueIncrement(0.01f);
    //TODO gibt es einen Max Value?


    /*Max Iterations Label and Box */
    new Label(controlParams, "max iterations:", "sans");
    auto intBoxMaxIter = new IntBox<int>(controlParams);
    intBoxMaxIter->setEditable(true);
    intBoxMaxIter->setFixedSize(Vector2i(80, 20));
    intBoxMaxIter->setValue(2);
    intBoxMaxIter->setUnits("int");
    intBoxMaxIter->setDefaultValue("0");
    intBoxMaxIter->setFontSize(16);
    intBoxMaxIter->setFormat("[1-9][0-9]*");
    intBoxMaxIter->setSpinnable(true);
    intBoxMaxIter->setMinValue(1);
    intBoxMaxIter->setValueIncrement(1);



    /*P Label and Box */
    new Label(controlParams, "p:", "sans");
    auto intBoxP = new IntBox<int>(controlParams);
    intBoxP->setEditable(true);
    intBoxP->setFixedSize(Vector2i(80, 20));
    intBoxP->setValue(2);
    intBoxP->setUnits("int");
    intBoxP->setDefaultValue("0");
    intBoxP->setFontSize(16);
    intBoxP->setFormat("[1-9][0-9]*");
    intBoxP->setSpinnable(true);
    intBoxP->setMinValue(1);
    intBoxP->setValueIncrement(1);


    /*Padding Label and Box */
    new Label(controlParams, "padding:", "sans");
    auto intBoxPad = new IntBox<int>(controlParams);
    intBoxPad->setEditable(true);
    intBoxPad->setFixedSize(Vector2i(80, 20));
    intBoxPad->setValue(2);
    intBoxPad->setUnits("int");
    intBoxPad->setDefaultValue("0");
    intBoxPad->setFontSize(16);
    intBoxPad->setFormat("[1-9][0-9]*");
    intBoxPad->setSpinnable(true);
    intBoxPad->setMinValue(1);
    intBoxPad->setValueIncrement(1);


    // ------ Create Optimize Button window ----------
    Window * controlsWindow = new Window(this, "");
    controlsWindow->setPosition(Vector2i(500, 550));
    controlsWindow->setFixedSize(Vector2i(500, 70));

    controlsWindow->setLayout(new GroupLayout());
    // Create optimization Button and set callback
    Button * optimizeButton = new Button(controlsWindow, "Optimize");


    // Start optimization on press button
    optimizeButton->setCallback([=] {

        uint resolutionFactor = (uint) intBoxResolution->value();
        uint maxIterations = (uint) intBoxMaxIter->value();
        int p = intBoxP->value();
        uint padding = (uint) intBoxPad->value();
        float alpha = floatBoxAlpha->value();
        float beta = floatBoxBeta->value();
        float lambda = floatBoxLambda->value();

        if (runOptimizationInLockStep) {
            this->runOptimization(maxIterations, p, padding, alpha, beta, lambda, resolutionFactor);
        }
        else {
            // run optimization in separate thread
            // fixme: currently not working
            if (!isOptimizing) {
                isOptimizing = true;

                thread optimizationThread([=] {
                    this->runOptimization(maxIterations, p, padding, alpha, beta, lambda, resolutionFactor);
                });
                optimizationThread.detach();
            }
        }

    });

    // ------ Create Result Image window ----------
    // Initialize image view with a placeholder image
    string placeholderFilename = "images/text00.png";
    currentResultImage = imread(placeholderFilename);
    if (!currentResultImage.data) {
        cout << "image '" << placeholderFilename << "' can not be found." << endl;
        exit(0);
    }

    // Convert OpenCV image to OpenGL texture wrapper object
    Texture * placeholderTexture = new Texture(currentResultImage, "result placeholder");

    // Create image window and view and display placeholder image
    resultImageWindow = new Window(this, "Result");
    resultImageWindow->setPosition(Vector2i(500, 20));
    resultImageWindow->setFixedSize(Vector2i(500, 500));
    resultImageWindow->setLayout(new GroupLayout());
    resultImageView = new ImageView(resultImageWindow, placeholderTexture->getTextureId());



    // ------ Create help window ------
    Window * helpWindow = new Window(this, "");
    helpWindow->setPosition(Vector2i(900, 650));
    helpWindow->setFixedSize(Vector2i(70, 70));
    helpWindow->setLayout(new GroupLayout());
    Button * helpButton = new Button(helpWindow, "?");


    // required by nanogui to render GUI
    performLayout();
}

void SuperResolutionApplication::runOptimization(uint maxIterations, int p, uint padding, float alpha, float beta,
                                                 float lambda, uint resolutionFactor) {

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

    //TODO create parameters from user input,
    //TODO check if input, else -> default values?
    optimizationParameters->setMaxIterations(maxIterations);
    optimizationParameters->setP(p);
    optimizationParameters->setPadding(padding);
    optimizationParameters->setAlpha(alpha);
    optimizationParameters->setBeta(beta);
    optimizationParameters->setLambda(lambda);
    optimizationParameters->setResolutionFactor(resolutionFactor);

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

