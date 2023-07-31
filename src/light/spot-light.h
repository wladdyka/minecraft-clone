//
// Created by wladdyka on 7/30/2023.
//

#ifndef OPENGLCOURSE_SPOT_LIGHT_H
#define OPENGLCOURSE_SPOT_LIGHT_H

#include "point-light.h"

class SpotLight: public PointLight {
public:
    SpotLight();
    ~SpotLight();

    SpotLight(glm::vec3 colour, GLfloat intensity, GLfloat diffuseIntensity, glm::vec3 position,
              glm::vec3 direction, GLfloat lin, GLfloat exp, GLfloat con, GLfloat edg);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
                  GLuint positionLocation, GLuint directionLocation, GLuint linearLocation,
                  GLuint exponentLocation, GLuint constantLocation, GLuint edgeLocation);

    void SetFlashlight(glm::vec3 pos, glm::vec3 dir);

private:
    glm::vec3 direction{};
    GLfloat edge{}, procEdge{};
};


#endif //OPENGLCOURSE_SPOT_LIGHT_H
