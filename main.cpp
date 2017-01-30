#include <iostream>

#include "SuperResolutionApplication.h"




using namespace std;
using namespace cv;


int main() {


    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<SuperResolutionApplication> app = new SuperResolutionApplication();
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