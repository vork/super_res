//
// Created by Paul Sanzenbacher on 30.01.17
//

#include "Texture.h"

using namespace std;
using namespace cv;
using namespace nanogui;

Texture::Texture(cv::Mat3b cvImage, string _textureName) : textureName(_textureName) {

    textureId = 0;

    int w = cvImage.cols;
    int h = cvImage.rows;

    Mat3b cvImageCropped;

    // OpenGL can't handle textures where a dimension is no multiple of 4, so it has to be cropped
    if (w % 4 != 0 || h % 4 != 0) {
        w = 4 * (w / 4);
        h = 4 * (h / 4);
        Rect croppedRegion(Point(0, 0), Size(w, h));
        cvImageCropped = Mat3b(croppedRegion.size());
        cvImage(croppedRegion).copyTo(cvImageCropped);
    }
    else {
        cvImageCropped = cvImage;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    GLint internalFormat = GL_RGB;
    GLint inputFormat = GL_BGR;

    const GLint pyramidLevel = 0;
    const GLint borderWidth = 0;

    glTexImage2D(GL_TEXTURE_2D, pyramidLevel, internalFormat, w, h, borderWidth, inputFormat, GL_UNSIGNED_BYTE, cvImageCropped.ptr());

    // set clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}