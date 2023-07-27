//
// Created by wladdyka on 7/25/2023.
//

#include "material.h"

Material::~Material() = default;

Material::Material() = default;

Material::Material(GLfloat intensity, GLfloat shine) {
    specularIntensity = intensity;
    shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}
