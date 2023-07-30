//
// Created by wladdyka on 7/24/2023.
//

#ifndef OPENGLCOURSE_LIGHT_H
#define OPENGLCOURSE_LIGHT_H


#include <glm/vec3.hpp>
#include <GL/glew.h>

class Light {
public:
    Light();
    ~Light();

    Light(glm::vec3 colour, GLfloat intensity, GLfloat diffuseIntensity);

protected:
    glm::vec3 colour{};
    GLfloat ambientIntensity{};
    GLfloat diffuseIntensity{};
};


#endif //OPENGLCOURSE_LIGHT_H
