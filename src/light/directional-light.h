//
// Created by wladdyka on 7/30/2023.
//

#ifndef OPENGLCOURSE_DIRECTIONAL_LIGHT_H
#define OPENGLCOURSE_DIRECTIONAL_LIGHT_H

#include "light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight();
    ~DirectionalLight();

    DirectionalLight(glm::vec3 colour, GLfloat intensity, GLfloat diffuseIntensity,
                     glm::vec3 diffuseDirection);

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                  GLfloat directionLocation);

private:
    glm::vec3 direction{};
};


#endif //OPENGLCOURSE_DIRECTIONAL_LIGHT_H
