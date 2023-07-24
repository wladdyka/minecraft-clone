//
// Created by wladdyka on 7/24/2023.
//

#include "light.h"

Light::Light(glm::vec3 newColour, GLfloat intensity) {
    colour = newColour;
    ambientIntensity = intensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation) {
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::Light() = default;
Light::~Light() = default;
