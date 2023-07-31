//
// Created by wladdyka on 7/30/2023.
//

#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>
#include "spot-light.h"

SpotLight::SpotLight(): PointLight() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    procEdge = cosf(glm::radians(edge));
}

SpotLight::~SpotLight() = default;

SpotLight::SpotLight(glm::vec3 colour, GLfloat intensity, GLfloat diffuseIntensity, glm::vec3 position,
                     glm::vec3 newDirection, GLfloat lin, GLfloat exp, GLfloat con, GLfloat edg) :
                     PointLight(colour, intensity, diffuseIntensity, position, con, lin, exp) {
    direction = glm::normalize(newDirection);
    edge = edg;
    procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
                         GLuint positionLocation, GLuint directionLocation, GLuint linearLocation,
                         GLuint exponentLocation, GLuint constantLocation, GLuint edgeLocation) {
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);

    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
    glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlashlight(glm::vec3 pos, glm::vec3 dir) {
    position = pos;
    direction = dir;
}
