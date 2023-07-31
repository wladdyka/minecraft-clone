//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_CAMERA_H
#define OPENGLCOURSE_CAMERA_H

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/fwd.hpp>

class Camera {
public:
    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat changeX, GLfloat changeY);
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();
    glm::mat4 calculateViewMatrix();
    ~Camera();
private:
    glm::vec3 position{};
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{};
    GLfloat yaw{};
    GLfloat pitch{};
    GLfloat movementSpeed{};
    GLfloat turnSpeed{};
    void update();
};


#endif //OPENGLCOURSE_CAMERA_H
