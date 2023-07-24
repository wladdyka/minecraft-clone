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
    Light(glm::vec3 colour, GLfloat intensity, glm::vec3 diffuseDirection, GLfloat diffuseIntensity);
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                  GLfloat directionLocation);
    ~Light();
private:
    glm::vec3 colour{};
    GLfloat ambientIntensity{};

    glm::vec3 direction{};
    GLfloat diffuseIntensity{};
};


#endif //OPENGLCOURSE_LIGHT_H
