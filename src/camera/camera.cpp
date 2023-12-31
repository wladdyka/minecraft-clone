//
// Created by wladdyka on 7/23/2023.
//

#include "camera.h"

#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() = default;

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
    GLfloat velocity = movementSpeed * deltaTime;

    if (keys[GLFW_KEY_W]) {
        position += front * velocity;
    }

    if (keys[GLFW_KEY_S]) {
        position -= front * velocity;
    }

    if (keys[GLFW_KEY_A]) {
        position -= right * velocity;
    }

    if (keys[GLFW_KEY_D]) {
        position += right * velocity;
    }
}

glm::mat4 Camera::calculateViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

glm::vec3 Camera::getCameraPosition() {
    return position;
}

glm::vec3 Camera::getCameraDirection() {
    return glm::normalize(front);
}

Camera::Camera() = default;
