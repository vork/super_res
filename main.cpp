#include <iostream>

#include "SuperResolutionApplication.h"
#include "ImageLoader.h"
#include "SuperResolution.h"


// use for debugging without the gui (easier to debug)
#define DEBUG_WITHOUT_GUI true


using namespace std;
using namespace cv;
using namespace Eigen;


Vector2i getScreenSize() {
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * screen = glfwGetVideoMode(monitor);
    Vector2i screenSize(screen->width, screen->height);
    return screenSize;
}

int main(int argc, char* argv[]) {

#if DEBUG_WITHOUT_GUI

    if (argc != 2) {
        cout << "one argument (image directory) needed when running in debug mode without GUI." << endl;
    }
    else {
        string imageDir = argv[1];

        // load images
        unique_ptr<ImageLoader> imageLoader(new ImageLoader());
        vector<Mat> _images = imageLoader->loadImages(imageDir);

        // convert to explicit Mat3f
        vector<Mat3f> images;
        for (auto _image : _images) {
            Mat3f image = _image;
            images.push_back(image);
        }
        printf("%i images loaded.\n", images.size());

        unique_ptr<ParameterFactory> parameterFactory(new ParameterFactory());

        unique_ptr<Parameters> parameters(parameterFactory->build(images));

        cout << parameters->str() << endl;

        const bool colored = false;
        unique_ptr<SuperResolution> superResolution(new SuperResolution(parameters.get(), colored));

        Mat3f result = superResolution->compute();
        imwrite("result.jpg", result);

    }

#else

    try {
        nanogui::init();

        Vector2i screenSize = getScreenSize();

        // use this to debug screen size
//        Vector2i screenSize(1000, 800);

        const float heightRelativeToWidth = 0.75f;
        const int maxWidth = 1024;
        const int maxHeight = 768;

        // choose widht as screen size (max. 1024 px)
        int width = min(maxWidth, screenSize.x());

        // adapt height
        int height = (int)(heightRelativeToWidth * width);

        // if too high for screen, specify height first, then adapt width
        if (height > maxHeight) {
            int height = screenSize.x();
            int width = (int)(height / heightRelativeToWidth);
        }

        Vector2i windowSize(width, height);

        /* scoped variables */ {
            nanogui::ref<SuperResolutionApplication> app = new SuperResolutionApplication(windowSize);
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();


    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#else
        std::cerr << error_msg << endl;
#endif
        return -1;
    }

#endif

    return 0;
}