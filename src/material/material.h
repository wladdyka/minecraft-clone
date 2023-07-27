//
// Created by wladdyka on 7/25/2023.
//

#ifndef OPENGLCOURSE_MATERIAL_H
#define OPENGLCOURSE_MATERIAL_H


#include <GL/glew.h>

class Material {
public:
    Material();
    Material(GLfloat intensity, GLfloat shine);

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

    ~Material();
private:
    GLfloat specularIntensity{};
    GLfloat shininess{};
};


#endif //OPENGLCOURSE_MATERIAL_H
