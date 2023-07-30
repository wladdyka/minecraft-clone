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

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation,
                  GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

private:
    glm::vec3 position{};
    GLfloat constant{}, linear{}, exponent{};
};


#endif //OPENGLCOURSE_POINT_LIGHT_H
