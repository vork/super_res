//
// Created by Paul Sanzenbacher on 29.01.17
//

#include "SuperResolutionApplication.h"

#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/combobox.h>
#include <nanogui/textbox.h>
#include <regex>

#include <thread>
#include <boost/filesystem/operations.hpp>
#include <nanogui/nanogui.h>
#include <monitor.h>

#include "ImageLoader.h"
#include "SimpleImageSet.h"
#include "SuperResolution.h"
#include "Texture.h"
#include "util.h"

using namespace nanogui;
using namespace std;
using namespace cv;

#define DIR_LABEL_PLACEHOLDER "not selected"


SuperResolutionApplication::SuperResolutionApplication(Eigen::Vector2i _windowSize) : nanogui::Screen(_windowSize, WINDOW_NAME) {


    windowSize = _windowSize;

    //GUI size dependencies, weird numbers come from dependency to pixel values, that were chosen before.

    const int windowWidth = windowSize.x();
    const int windowHeight = windowSize.y();

    const int marginSpace = windowWidth * 0.0244;

    const int windowWidthLeft = windowWidth * 0.4394;

    const int windowWidthRight = windowWidth - (3 * marginSpace) - windowWidthLeft; //width of the right parts of the window

    const int windowHeightUpper = windowHeight * 0.39;

    const int parameterBoxHeight = windowHeight * 0.026;
    const int parameterBoxWidth = windowWidth * 0.1171;

    const int buttonHeight = windowHeight * 0.0911;

    const int widthPosOptimize = windowHeight-buttonHeight-marginSpace; //width for the Position of the window for the optimize Button


    isOptimizing = false; //variable that determines if optimization process is running.

    // ------ Create Low Resolution Image Panel ----------
    Window * fileWindow = new Window(this, "Low Resolution Images");
    fileWindow->setPosition(Vector2i(marginSpace, marginSpace));
    fileWindow->setFixedSize(Vector2i(windowWidthLeft, windowHeightUpper));
    fileWindow->setLayout(new GroupLayout());


    // File Dialog
    new Label(fileWindow, "File Dialog: ", "sans-bold");

    Widget * directoryWidget = new Widget(fileWindow);
    directoryWidget->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, 6));


    Button * openDirButton = new Button(directoryWidget, "Open Directory");
//    openDirButton->setFixedSize(Vector2i(300, 35));
    openDirButton->setTooltip("Click this button to choose the directory of the blurred images.");
    openDirButton->setCallback([this] {
        const string &directoryDialogResult = directory_dialog();

        // make sure string is not empty (directory dialog cancelled)
        if (directoryDialogResult.size() > 0) {
            this->directoryPath = directoryDialogResult + "/";
//        this->directoryLabel->setCaption(directoryPath);
            this->loadImages();
        }
    });

    Button * saveResultButton = new Button(directoryWidget, "Save Result");
    saveResultButton->setCallback([this] {
        vector<pair<string, string>> allowedFiletypes({
            {"png", "Portable Network Graphics"},
            {"jpg", "JPEG"}
        });
        string saveFilename = file_dialog(allowedFiletypes, true);
        if (currentResultImage.empty()) {
            string warningTitle = "No result image";
            string warningMessage = "Run optimization before saving the result image.";
            MessageDialog * dialog = new MessageDialog(this, MessageDialog::Type::Warning, warningTitle, warningMessage);
            (void)dialog; // silence unused variable warning
        }
        else {

            namespace fs = boost::filesystem;
            fs::path filenamePath(saveFilename);

            string extension = filenamePath.extension().string();

            // check if valid extension
            bool validExtension = false;
            for (auto filetype : allowedFiletypes) {
                if (filetype.first == extension) {
                    validExtension = true;
                    break;
                }
            }

            // set jpeg by default
            if (!validExtension) {
                saveFilename = filenamePath.replace_extension("jpg").string();
                printf("Invalid file extension. It is replaced by '.jpg'.\n");
            }

            imwrite(saveFilename, currentResultImage);
        }
    });


//    directoryPath = DIR_LABEL_PLACEHOLDER;
//    directoryLabel = new Label(fileWindow, directoryPath, "sans-bold");


    /* Image Panel */

    scrollPanel = new VScrollPanel(fileWindow);
    const int imagePanelHeight = windowHeight * 0.234;
    scrollPanel->setFixedHeight(imagePanelHeight);
    imagePanel = new ImagePanel(scrollPanel);
    imagePanel->setFixedHeight(imagePanelHeight);
    imagePanel->setCallback([this](int index) {
        if (index >= 0 && index < (int)sourceImages.size()) {
            displayImage(sourceImages[index]);
        }
        else {
            printf("Image panel index %i is not valid for %i source images.\n", index, (int)sourceImages.size());
        }
    });

    // hide scroll panel in the beginning
    scrollPanel->setVisible(false);


    // ------ Create Control Parameter Window ----------
    Window * parameterWindow = new Window(this, "Control Parameters");
    parameterWindow->setPosition(Vector2i(marginSpace, windowHeightUpper + (2 * marginSpace)));
    parameterWindow->setFixedSize(Vector2i(windowWidthLeft, windowHeight - (windowHeightUpper + (3 * marginSpace))));

    GridLayout *layout =
            new GridLayout(Orientation::Horizontal, 2,
                           Alignment::Middle, 15, 5);
    layout->setColAlignment(
            { Alignment::Maximum, Alignment::Fill });
    layout->setSpacing(0, 10);
    parameterWindow->setLayout(layout);


    /*Resolution Factor Label and Box */
    Label * labelResolution = new Label(parameterWindow, "resolution factor:", "sans");
    const string ttResolution = "An integer value between 2 and 4 is recommended.";
    labelResolution->setTooltip(ttResolution);
    auto intBoxResolution = new IntBox<int>(parameterWindow);
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
    intBoxResolution->setMinMaxValues(1, 16);


    /* Alpha Label and Box */
    Label * labelAlpha = new Label(parameterWindow, "alpha:", "sans");
    const string ttAlpha = "Set alpha.";
    labelAlpha->setTooltip(ttAlpha);
    auto floatBoxAlpha = new FloatBox<float>(parameterWindow);
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
    floatBoxAlpha->setMinMaxValues(0.001f, 1000.0f);



    /* Beta Label and Box */
    Label * labelBeta = new Label(parameterWindow, "beta:", "sans");
    const string ttBeta = "Set beta.";
    labelBeta->setTooltip(ttBeta);
    auto floatBoxBeta = new FloatBox<float>(parameterWindow);
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
    floatBoxBeta->setMinMaxValues(0.001f, 1000.0f);


    /* Lambda Label and Box */
    Label * labelLambda = new Label(parameterWindow, "lambda:", "sans");
    const string ttLambda = "Set Lambda";
    labelLambda->setTooltip(ttLambda);
    auto floatBoxLambda = new FloatBox<float>(parameterWindow);
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
    floatBoxLambda->setMinMaxValues(0.001f, 1000.0f);


    /*Max Iterations Label and Box */
    Label * labelMaxIter = new Label(parameterWindow, "max iterations:", "sans");
    const string ttMaxIter = "Choose the number of iterations. An integer between 5 and 25 is recommended.";
    labelMaxIter->setTooltip(ttMaxIter);
    auto intBoxMaxIter = new IntBox<int>(parameterWindow);
    intBoxMaxIter->setEditable(true);
    intBoxMaxIter->setFixedSize(Vector2i(parameterBoxWidth, parameterBoxHeight));
    intBoxMaxIter->setValue(20);
    intBoxMaxIter->setUnits("int");
    intBoxMaxIter->setDefaultValue("0");
    intBoxMaxIter->setFontSize(16);
    intBoxMaxIter->setFormat("[1-9][0-9]*");
    intBoxMaxIter->setSpinnable(true);
    intBoxMaxIter->setMinValue(1);
    intBoxMaxIter->setValueIncrement(1);
    intBoxMaxIter->setTooltip(ttMaxIter);
    intBoxMaxIter->setMinMaxValues(1, 100);



    /*P Label and Box */
    Label * labelP = new Label(parameterWindow, "p:", "sans");
    const string ttP = "Set p.";
    labelP->setTooltip(ttP);
    auto intBoxP = new IntBox<int>(parameterWindow);
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
    intBoxP->setMinMaxValues(1, 20);


    /*Padding Label and Box */
    Label * labelPadding = new Label(parameterWindow, "padding:", "sans");
    const string ttPadding = "Set padding.";
    labelPadding->setTooltip(ttPadding);
    auto intBoxPad = new IntBox<int>(parameterWindow);
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
    intBoxPad->setMinMaxValues(0, 20);


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


    hiResColorCheckBox = new CheckBox(controlsWindow, "Compute colored high resolution image");
    hiResColorCheckBox->setChecked(hiResColor);
    hiResColorCheckBox->setTooltip("Only enabled for colored images");
    hiResColorCheckBox->setCallback([this](bool state) {
        this->hiResColor= state;
    });

    // disable checkbox at the beginning
    hiResColorCheckBox->setEnabled(false);

//    CheckBox * showIterationResultsCheckBox = new CheckBox(controlsWindow, "Show intermediate results");
//    showIterationResultsCheckBox->setChecked(showIterationResults);
//    showIterationResultsCheckBox->setCallback([this](bool state) {
//        this->showIterationResults = state;
//
//        // show slowdown message
//        if (state) {
//            string warningTitle = "Optimization Slowdown";
//            string warningMessage = "Note: when showing intermediate results, the optimization process will take longer.";
//            MessageDialog * dialog = new MessageDialog(this, MessageDialog::Type::Warning, warningTitle, warningMessage);
//        }
//    });

    // ------ Create Result Image window ----------

    // Initialize image view with a placeholder image
    currentResultImage = Mat3b(Size(0, 0));

    // Convert OpenCV image to OpenGL texture wrapper object
    Texture * placeholderTexture = new Texture(currentResultImage, "result placeholder");

    // Create image window and view and display placeholder image
    resultImageWindow = new Window(this, "Result");
    resultImageWindow->setPosition(Vector2i(windowWidth * 0.488, marginSpace));

    resultImageWindow->setFixedSize(Vector2i(windowWidthRight, windowHeight - (4 * marginSpace) - buttonHeight));
    resultImageWindow->setTooltip("The result image is shown in this window.");
    resultImageWindow->setLayout(new GroupLayout());
    resultImageView = new ImageView(resultImageWindow, placeholderTexture->getTextureId());
    const int imageViewHeight = resultImageWindow->fixedHeight() - (windowHeight * 0.078) ;
    resultImageView->setFixedHeight(imageViewHeight);


    // ------ Create help window ------
    Window * helpWindow = new Window(this, "");
    helpWindow->setPosition(Vector2i(windowWidth * 0.898, widthPosOptimize));

    helpWindow->setFixedSize(Vector2i(buttonHeight, buttonHeight));
    helpWindow->setLayout(new GroupLayout());
    helpWindow->setTooltip("Click to get help.");
    Button * helpButton = new Button(helpWindow, "?");

    helpButton->setCallback([&] {
        auto dlg = new MessageDialog(this, MessageDialog::Type::Information, "Help", "Tooltips are provided for every button"
                "and input field. These get visible if you stay longer with the mouse.");
        dlg->setCallback([](int result) { cout << "Dialog closed result: " << result << endl; });
    });

    // required by nanogui to render GUI
    performLayout();

}

void SuperResolutionApplication::runOptimization(uint maxIterations, int p, uint padding, float alpha, float beta,
                                                 float lambda, uint resolutionFactor) {

    if (sourceImages.size() < 2) {
        string warningTitle = "Too few images";
        string warningMessage = "At least 2 images needed to run optimization.";
        MessageDialog * dialog = new MessageDialog(this, MessageDialog::Type::Warning, warningTitle, warningMessage);
        (void)dialog; // silence unused variable warning
        return;
    }

    // convert images to Mat1f
    vector<Mat3f> rgbImages;
    for (Mat image : sourceImages) {
        Mat3f rgbImage;
        image.convertTo(rgbImage, CV_32FC3);
        rgbImages.push_back(rgbImage);
    }

    // create default parameter set
    ParameterFactory factory;

    factory.setMaxIterations(maxIterations);
    factory.setP(p);
    factory.setPadding(padding);
    factory.setAlpha(alpha);
    factory.setBeta(beta);
    factory.setLambda(lambda);
    factory.setResolutionFactor(resolutionFactor);


    optimizationParameters = factory.build(rgbImages);

    // run super-resolution algorithm
    SuperResolution * superResolution = new SuperResolution(optimizationParameters, hiResColor);

//    superResolution->setIterationCallback([this](Mat1f intermediateResult) {
//        if (this->showIterationResults) {
//            Mat1b bResult;
//            intermediateResult.convertTo(bResult, CV_8UC1);
//            cvtColor(bResult, this->currentResultImage, CV_GRAY2BGR);
//            this->displayImage(this->currentResultImage);
//        }
//    });

    Mat3f hrImage = superResolution->compute();

    // convert to byte representation
    Mat3b bResult;
    hrImage.convertTo(bResult, CV_8UC3);

    // convert from rgb to bgr
    cvtColor(bResult, currentResultImage, CV_RGB2BGR);

    // only when image has 3 channels
    //TODO überprüfen auf gray image (gleiche werte der Channel)
//    if (sourceImages[0].channels() == 3){
//        if (hiResColor) {
//            currentResultImage = superResolution->extractColorInformation();
//        } else {
//            // convert result to displayable format
//            Mat1b bResult;
//            hrImage.convertTo(bResult, CV_8UC1);
//            cvtColor(bResult, currentResultImage, CV_GRAY2BGR);
//        }
//    } else {
//        string warningTitle = "too few channels";
//        string warningMessage = "3 channels are necessary for color image.";
//        MessageDialog * dialog = new MessageDialog(this, MessageDialog::Type::Warning, warningTitle, warningMessage);
//        (void)dialog; // silence unused variable warning
//        return;
//    }

    displayImage(currentResultImage);

    isOptimizing = false;
}

void SuperResolutionApplication::displayImage(Mat3b image) const {

    Texture * texture = new Texture(image, "image");
    resultImageView->bindImage(texture->getTextureId());

    // compute scale
    Vector2i imageViewSize = resultImageView->size();
    Size imageSize = image.size();
    float scale = fminf((float)imageViewSize.y() / (float)imageSize.height, (float)imageViewSize.x() / (float)imageSize.width);

    resultImageView->setScale(scale);
    resultImageView->center();
}

void SuperResolutionApplication::draw(NVGcontext *ctx) {

    /* Draw the user interface */
    Screen::draw(ctx);
}

void SuperResolutionApplication::drawContents() {

//    mShader.bind();
}

void SuperResolutionApplication::loadImages() {

    sourceImages.clear();

    bool isDirectoryValid = true;
    string warningTitle, warningMessage;

    if(directoryPath.empty() || directoryPath == DIR_LABEL_PLACEHOLDER){
        isDirectoryValid = false;
        warningTitle = "No directory specified";
        warningMessage = "Please specify an image directory before optimizing.";

    }
    else if (!boost::filesystem::exists(directoryPath)) {
        isDirectoryValid = false;
        warningTitle = "Directory not found";
        warningMessage = "The path '" + this->directoryPath + "' is not a valid directory.";
    }
    else {
        // load images
        unique_ptr<ImageLoader> imageLoader(new ImageLoader());
        vector<string> filenames;
        sourceImages = imageLoader->loadImages(directoryPath, filenames);

        areSourceImagesColored = true;
        for (Mat image : sourceImages) {
            if (isGrayscaleImage(image)) {
                areSourceImagesColored = false;
                break;
            }
        }

        // enable / disable checkbox based on whether the source images are colored or not
        hiResColorCheckBox->setEnabled(areSourceImagesColored);

        // make sure there is at least one image
        if (sourceImages.size() == 0) {
            isDirectoryValid = false;
            warningTitle = "No images found";
            warningMessage = "No images found in directory '" + this->directoryPath + "'.";
        }
        else {
            // make sure all images are the same size
            Size size = sourceImages[0].size();
            for (unsigned int i = 1; i < sourceImages.size(); i++) {
                if (sourceImages[i].size() != size) {

                    isDirectoryValid = false;
                    warningTitle = "Invalid image dimensions";
                    warningMessage = "All images need to have the be the exact same size.\n";
                    warningMessage += "Size of '" + filenames[0] + "' is different from ";
                    warningMessage += "size of '" + filenames[i] + "'.";

                    break;
                }
            }
        }
    }

    // print warning message if directory is not valid and leave optimization
    if (!isDirectoryValid) {
        MessageDialog * dialog = new MessageDialog(this, MessageDialog::Type::Warning, warningTitle, warningMessage);
        (void)dialog; // silence unused variable warning
        return;
    }

    // add images to image panel
    vector<pair<int, string>> panelImages;

    for (Mat sourceImage : sourceImages) {
        Mat alphaImage;
        cvtColor(sourceImage, alphaImage, CV_BGR2RGBA);
        int w = alphaImage.cols;
        int h = alphaImage.rows;
        int imageFlags = 0;
        unsigned char * data = (unsigned char *)alphaImage.data;
        int imageId = nvgCreateImageRGBA(mNVGContext, w , h, imageFlags, data);
        string imageName = "test";
        panelImages.push_back(pair<int, string>(imageId, imageName));
    }

    imagePanel->setImages(panelImages);
    scrollPanel->setVisible(true);
    performLayout();

    // set first image in result view
    displayImage(sourceImages[0]);

}

