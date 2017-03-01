#include <iostream>

#include "SuperResolutionApplication.h"


using namespace std;
using namespace cv;
using namespace Eigen;


Vector2i getScreenSize() {
    GLFWmonitor * monitor;
    const GLFWvidmode * screen;
    screen = glfwGetVideoMode(monitor);
    Vector2i screenSize(screen->width, screen->height);
    return screenSize;
}

int main() {


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

    return 0;
}