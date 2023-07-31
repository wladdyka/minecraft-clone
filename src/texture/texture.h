//
// Created by wladdyka on 7/24/2023.
//

#ifndef OPENGLCOURSE_TEXTURE_H
#define OPENGLCOURSE_TEXTURE_H

#include <GL/gl.h>
#include "stb_image.h"

class Texture {
public:
    Texture();
    Texture(const char *filePath);
    bool LoadTexture();
    bool LoadTextureAlpha();
    void UseTexture();
    void ClearTexture();
    ~Texture();
private:
    GLuint textureId{};
    int width{}, height{}, bitDepth{};
    char* filePath{};
};


#endif //OPENGLCOURSE_TEXTURE_H
