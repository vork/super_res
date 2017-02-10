//
// Created by Paul Sanzenbacher on 29.01.17
//

#include "SuperResolutionApplication.h"
//

#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <nanogui/combobox.h>
#include <nanogui/textbox.h>
#include <regex>


#include <thread>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "SuperResolution.h"
#include "Texture.h"

using namespace nanogui;
using namespace std;
using namespace cv;


SuperResolutionApplication::SuperResolutionApplication() : nanogui::Screen(SCREEN_RES, WINDOW_NAME) {

    //GUI size dependenciess
    int screenWidth = screenRes.x();
    int screenHeight = screenRes.y();

    int marginSpace = 25;

    int windowWidthLeft = 450;
    int windowWidthRight = screenWidth - (3 * marginSpace) - windowWidthLeft;

    int windowHeightUpper = 300;

    int parameterBoxHeight = 20;
    int parameterBoxWidth = 120;

    int buttonHeight = 70;

    int widthPosOptimize = screenHeight-buttonHeight-marginSpace;


    isOptimizing = false;

    // ------ Create Low Resolution Image Panel ----------
    Window * lowResImgs = new Window(this, "Low Resolution Images");
    lowResImgs->setPosition(Vector2i(marginSpace, marginSpace));
    lowResImgs->setFixedSize(Vector2i(windowWidthLeft, windowHeightUpper));
    lowResImgs->setLayout(new GroupLayout());


    // File Dialog
    new Label(lowResImgs, "File Dialog: ", "sans-bold");

    Button * b = new Button(lowResImgs, "Choose directory in file system");
    b->setPosition(Vector2i(100, 150));
    b->setFixedSize(Vector2i(300, 50));
    b->setCallback([=] {

        directoryPath = directory_dialog() + "/";

        Label * directoryLabel = new Label(lowResImgs, directoryPath, "sans-bold");
        directoryLabel->setPosition(Vector2i(80, marginSpace + 21 ));

    });



    // ------ Create Control Parameter Window ----------
    Window * controlParams = new Window(this, "Control Parameters");
    controlParams->setPosition(Vector2i(marginSpace, windowHeightUpper + (2 * marginSpace)));
    controlParams->setFixedSize(Vector2i(windowWidthLeft, screenHeight - (windowHeightUpper + (3 * marginSpace))));

    GridLayout *layout =
            new GridLayout(Orientation::Horizontal, 2,
                           Alignment::Middle, 15, 5);
    layout->setColAlignment(
            { Alignment::Maximum, Alignment::Fill });
    layout->setSpacing(0, 10);
    controlParams->setLayout(layout);


    /*Resolution Factor Label and Box */
    Label * labelResolution = new Label(controlParams, "resolution factor:", "sans");
    const string ttResolution = "Choose an integer value that is a power of 2.";
    labelResolution->setTooltip(ttResolution);
    auto intBoxResolution = new IntBox<int>(controlParams);
    intBoxResolution->setEditable(true);
    intBoxResolution->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    intBoxResolution->setValue(2);
    intBoxResolution->setUnits("int");
    intBoxResolution->setDefaultValue("2");
    intBoxResolution->setFontSize(16);
    intBoxResolution->setFormat("[1-9][0-9]*");
    intBoxResolution->setSpinnable(true);
    intBoxResolution->setMinValue(1);
    intBoxResolution->setValueIncrement(1);
    intBoxResolution->setTooltip(ttResolution);
    //TODO: Max Value setzen?


    /* Alpha Label and Box */
    Label * labelAlpha = new Label(controlParams, "alpha:", "sans");
    const string ttAlpha = "Set alpha.";
    labelAlpha->setTooltip(ttAlpha);
    auto floatBoxAlpha = new FloatBox<float>(controlParams);
    floatBoxAlpha->setEditable(true);
    floatBoxAlpha->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    floatBoxAlpha->setValue(0.7f);
    floatBoxAlpha->setUnits("float");
    floatBoxAlpha->setDefaultValue("0.7");
    floatBoxAlpha->setFontSize(16);
    floatBoxAlpha->setFormat("[0-9].[0-9][0-9]*");
    floatBoxAlpha->setSpinnable(true);
    floatBoxAlpha->setMinValue(0.0f);
    floatBoxAlpha->setValueIncrement(0.1f);
    floatBoxAlpha->setTooltip(ttAlpha);
    //TODO gibt es einen Max Value?



    /* Beta Label and Box */
    Label * labelBeta = new Label(controlParams, "beta:", "sans");
    const string ttBeta = "Set beta.";
    labelBeta->setTooltip(ttBeta);
    auto floatBoxBeta = new FloatBox<float>(controlParams);
    floatBoxBeta->setEditable(true);
    floatBoxBeta->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    floatBoxBeta->setValue(1.0f);
    floatBoxBeta->setUnits("float");
    floatBoxBeta->setDefaultValue("1.0");
    floatBoxBeta->setFontSize(16);
    floatBoxBeta->setFormat("[0-9].[0-9][0-9]*");
    floatBoxBeta->setSpinnable(true);
    floatBoxBeta->setMinValue(0.0f);
    floatBoxBeta->setValueIncrement(0.1f);
    floatBoxBeta->setTooltip(ttBeta);
    //TODO gibt es einen Max Value?


    /* Lambda Label and Box */
    Label * labelLambda = new Label(controlParams, "lambda:", "sans");
    const string ttLambda = "Set Lambda";
    labelLambda->setTooltip(ttLambda);
    auto floatBoxLambda = new FloatBox<float>(controlParams);
    floatBoxLambda->setEditable(true);
    floatBoxLambda->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    floatBoxLambda->setValue(0.04f);
    floatBoxLambda->setUnits("float");
    floatBoxLambda->setDefaultValue("0.04");
    floatBoxLambda->setFontSize(16);
    floatBoxLambda->setFormat("[0-9].[0-9][0-9]*");
    floatBoxLambda->setSpinnable(true);
    floatBoxLambda->setMinValue(0.0f);
    floatBoxLambda->setValueIncrement(0.01f);
    floatBoxLambda->setTooltip(ttLambda);
    //TODO gibt es einen Max Value?


    /*Max Iterations Label and Box */
    Label * labelMaxIter = new Label(controlParams, "max iterations:", "sans");
    const string ttMaxIter = "Choose the number of maximum iterations. An integer between 2 and 25 is recommended.";
    labelMaxIter->setTooltip(ttMaxIter);
    auto intBoxMaxIter = new IntBox<int>(controlParams);
    intBoxMaxIter->setEditable(true);
    intBoxMaxIter->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    intBoxMaxIter->setValue(2);
    intBoxMaxIter->setUnits("int");
    intBoxMaxIter->setDefaultValue("0");
    intBoxMaxIter->setFontSize(16);
    intBoxMaxIter->setFormat("[1-9][0-9]*");
    intBoxMaxIter->setSpinnable(true);
    intBoxMaxIter->setMinValue(1);
    intBoxMaxIter->setValueIncrement(1);
    intBoxMaxIter->setTooltip(ttMaxIter);



    /*P Label and Box */
    Label * labelP = new Label(controlParams, "p:", "sans");
    const string ttP = "Set p.";
    labelP->setTooltip(ttP);
    auto intBoxP = new IntBox<int>(controlParams);
    intBoxP->setEditable(true);
    intBoxP->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    intBoxP->setValue(2);
    intBoxP->setUnits("int");
    intBoxP->setDefaultValue("1");
    intBoxP->setFontSize(16);
    intBoxP->setFormat("[1-9][0-9]*");
    intBoxP->setSpinnable(true);
    intBoxP->setMinValue(1);
    intBoxP->setValueIncrement(1);
    intBoxP->setTooltip(ttP);


    /*Padding Label and Box */
    Label * labelPadding = new Label(controlParams, "padding:", "sans");
    const string ttPadding = "Set padding.";
    labelPadding->setTooltip(ttPadding);
    auto intBoxPad = new IntBox<int>(controlParams);
    intBoxPad->setEditable(true);
    intBoxPad->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    intBoxPad->setValue(2);
    intBoxPad->setUnits("int");
    intBoxPad->setDefaultValue("0");
    intBoxPad->setFontSize(16);
    intBoxPad->setFormat("[1-9][0-9]*");
    intBoxPad->setSpinnable(true);
    intBoxPad->setMinValue(1);
    intBoxPad->setValueIncrement(1);
    intBoxPad->setTooltip(ttPadding);


    // ------ Create Optimize Button window ----------
    Window * controlsWindow = new Window(this, "");
    controlsWindow->setPosition(Vector2i(windowWidthLeft + (2 * marginSpace), widthPosOptimize));
    controlsWindow->setFixedSize(Vector2i(windowWidthRight - buttonHeight - marginSpace - marginSpace, buttonHeight));

    controlsWindow->setLayout(new GroupLayout());
    // Create optimization Button and set callback
    Button * optimizeButton = new Button(controlsWindow, "Optimize");
    optimizeButton->setTooltip("Click this button to start the optimization process. The result is shown in the window above.");


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
    resultImageWindow->setPosition(Vector2i(500, marginSpace));
    resultImageWindow->setFixedSize(Vector2i(windowWidthRight, screenHeight - (4 * marginSpace) - buttonHeight));
    resultImageWindow->setTooltip("The result image is shown in this window.");
    resultImageWindow->setLayout(new GroupLayout());
    resultImageView = new ImageView(resultImageWindow, placeholderTexture->getTextureId());



    // ------ Create help window ------
    Window * helpWindow = new Window(this, "");
    helpWindow->setPosition(Vector2i(920, widthPosOptimize));
    helpWindow->setFixedSize(Vector2i(buttonHeight, buttonHeight));
    helpWindow->setLayout(new GroupLayout());
    helpWindow->setTooltip("Click to get help.");
    Button * helpButton = new Button(helpWindow, "?");


    // required by nanogui to render GUI
    performLayout();
}

void SuperResolutionApplication::runOptimization(uint maxIterations, int p, uint padding, float alpha, float beta,
                                                 float lambda, uint resolutionFactor) {

    if(directoryPath.empty()){
        cout << "No image directory path found.";
    }

    //TODO
//    string imageDirectoryPath = directoryPath;



    ImageLoader * imageLoader = new ImageLoader();
    vector<Mat> images = imageLoader->loadImages(directoryPath);

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

    //Check if input is not null
    (maxIterations == NULL) ? optimizationParameters->setMaxIterations(2) : optimizationParameters->setMaxIterations(maxIterations);
    (p == NULL) ? optimizationParameters->setP(2) : optimizationParameters->setP(p);
    (padding == NULL) ? optimizationParameters->setPadding(2) : optimizationParameters->setPadding(padding);
    (alpha == NULL) ? optimizationParameters->setAlpha(0.7f) : optimizationParameters->setAlpha(alpha);
    (beta == NULL) ? optimizationParameters->setBeta(1.0f) : optimizationParameters->setBeta(beta);
    (lambda == NULL) ? optimizationParameters->setLambda(0.04f) : optimizationParameters->setLambda(lambda);
    (resolutionFactor == NULL) ? optimizationParameters->setResolutionFactor(2) : optimizationParameters->setResolutionFactor(resolutionFactor);

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

