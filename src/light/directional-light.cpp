//
// Created by wladdyka on 7/30/2023.
//

#include "directional-light.h"

DirectionalLight::DirectionalLight(glm::vec3 newColour, GLfloat intensity, GLfloat newDiffuseIntensity,
                                   glm::vec3 diffuseDirection) : Light(newColour, intensity, newDiffuseIntensity) {
    direction = diffuseDirection;
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                     GLfloat directionLocation) {
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::DirectionalLight() : Light() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::~DirectionalLight() = default;;