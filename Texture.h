//
// Created by Paul Sanzenbacher on 30.01.17
//

/**
 * This class wraps OpenGL textures and enables creation of a texture from an OpenCV image
 */

#ifndef SUPER_RES_TEXTURE_H
#define SUPER_RES_TEXTURE_H

#include <nanogui/glutil.h>
#include <cv.hpp>
#include <string>

class Texture {
protected:
    GLuint textureId;
    std::string textureName;
public:
    Texture(cv::Mat3b cvImage, std::string _textureName = "<texture>");

    ~Texture() {
        if (textureId) {
            glDeleteTextures(1, &textureId);
        }
    };

    GLuint getTextureId() const {
        return textureId;
    }

    const std::string &getTextureName() const {
        return textureName;
    }
};


#endif //SUPER_RES_TEXTURE_H
