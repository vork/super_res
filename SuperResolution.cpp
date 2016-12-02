//
// Created by Paul Sanzenbacher on 30.11.16
//

#include "SuperResolution.h"

using namespace cv;
using namespace std;

SuperResolution::SuperResolution(Parameters *parameters, ImageSet *imageSet) {
    this->parameters = parameters;
    this->imageSet = imageSet;
}


Mat SuperResolution::compute() {

    for (unsigned int iteration = 0; iteration < parameters->getMaxIterations(); iteration++) {

    }


}