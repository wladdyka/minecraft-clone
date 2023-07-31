//
// Created by wladdyka on 7/24/2023.
//

#include <GL/glew.h>
#include <cstring>
#include "texture.h"

Texture::Texture() = default;

void Texture::UseTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

Texture::Texture(const char * newFilePath) {
    filePath = strdup(newFilePath);
}

void Texture::ClearTexture() {
    glDeleteTextures(1, &textureId);
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    filePath = {};
}

bool Texture::LoadTexture() {
    unsigned char* textureData = stbi_load(filePath, &width, &height, &bitDepth, 0);
    if (!textureData) {
        printf("Failed to find: %s\n", filePath);
        return false;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);

    return true;
}

Texture::~Texture() {
    ClearTexture();
}

bool Texture::LoadTextureAlpha() {
    unsigned char* textureData = stbi_load(filePath, &width, &height, &bitDepth, 0);
    if (!textureData) {
        printf("Failed to find: %s\n", filePath);
        return false;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);

    return true;
};
