//
// Created by wladdyka on 7/24/2023.
//

#include "light.h"

Light::Light(glm::vec3 newColour, GLfloat intensity, GLfloat newDiffuseIntensity) {
    colour = newColour;
    ambientIntensity = intensity;
    diffuseIntensity = newDiffuseIntensity;
}

Light::Light() {
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 1.0f;
};
Light::~Light() = default;
