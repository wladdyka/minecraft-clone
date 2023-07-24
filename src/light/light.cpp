//
// Created by wladdyka on 7/24/2023.
//

#include "light.h"

Light::Light(glm::vec3 newColour, GLfloat intensity, glm::vec3 diffuseDirection, GLfloat newDiffuseIntensity) {
    colour = newColour;
    ambientIntensity = intensity;
    direction = diffuseDirection;
    diffuseIntensity = newDiffuseIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                     GLfloat directionLocation) {
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::Light() {
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;

    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    diffuseIntensity = 1.0f;
};
Light::~Light() = default;
