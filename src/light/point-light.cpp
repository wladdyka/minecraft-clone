//
// Created by wladdyka on 7/30/2023.
//

#include "point-light.h"

PointLight::PointLight(): Light() {
    position = glm::vec3(glm::vec3(0.0f, 0.0f, 0.0f));
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::~PointLight() = default;

PointLight::PointLight(glm::vec3 colour, GLfloat intensity, GLfloat diffuseIntensity, glm::vec3 newPosition, GLfloat con,
                       GLfloat lin, GLfloat exp): Light(colour, intensity, diffuseIntensity) {
    position = glm::vec3(newPosition);
    constant = con;
    linear = lin;
    exponent = exp;
}

void
PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                     GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation,
                     GLfloat exponentLocation) {
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}
