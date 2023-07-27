//
// Created by wladdyka on 7/23/2023.
//

#include <cstdio>

#include "window.h"

Window::Window() {
    width = 800;
    height = 600;
}

int Window::Init() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr);
    if (!mainWindow) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

GLfloat Window::aspectRation() const {
    return (GLfloat)bufferWidth / (GLfloat)bufferHeight;
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
    auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    auto anyKey = key >= 0 && key < 1024;
    if (anyKey && action == GLFW_PRESS) {
        theWindow->keys[key] = true;
        printf("Pressed: %s\n", glfwGetKeyName(key, 0));
    } else if (anyKey && action == GLFW_RELEASE) {
        theWindow->keys[key] = false;
        printf("Release: %s\n", glfwGetKeyName(key, 0));
    }
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleMouse(GLFWwindow *window, double positionX, double positionY) {
    auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (theWindow->mouseMovementInitialized) {
        theWindow->mouseLastX = positionX;
        theWindow->mouseLastY = positionY;
        theWindow->mouseMovementInitialized = false;
    }

    theWindow->mouseChangeX = positionX - theWindow->mouseLastX;
    theWindow->mouseChangeY = theWindow->mouseLastY - positionY;

    theWindow->mouseLastX = positionX;
    theWindow->mouseLastY = positionY;

    // printf("x:%.6f, y:%.6f\n", theWindow->mouseChangeX, theWindow->mouseChangeY);;
}

GLfloat Window::getMouseChangeX() {
    GLfloat change = mouseChangeX;
    mouseChangeX = 0;

    return change;
}

GLfloat Window::getMouseChangeY() {
    GLfloat change = mouseChangeY;
    mouseChangeY = 0;

    return change;
}
