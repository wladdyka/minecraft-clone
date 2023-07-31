//
// Created by wladdyka on 7/30/2023.
//

#ifndef OPENGLCOURSE_POINT_LIGHT_H
#define OPENGLCOURSE_POINT_LIGHT_H


#include "light.h"

class PointLight : public Light {
public:
    PointLight();
    ~PointLight();

    PointLight(glm::vec3 colour, GLfloat intensity, GLfloat diffuseIntensity,
               glm::vec3 position, GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
                  GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

protected:
    glm::vec3 position{};
    GLfloat constant{}, linear{}, exponent{};
};


#endif //OPENGLCOURSE_POINT_LIGHT_H
