//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_WINDOW_H
#define OPENGLCOURSE_WINDOW_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();
    int Init();
    GLfloat aspectRation() const;
    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
    void swapBuffers() { glfwSwapBuffers(mainWindow); }
    bool* getKeys() { return keys; }
    GLfloat getMouseChangeX();
    GLfloat getMouseChangeY();

private:
    GLint width, height;
    GLint bufferWidth{}, bufferHeight{};
    GLFWwindow* mainWindow{};
    bool keys[1024]{};
    bool mouseMovementInitialized = true;
    GLfloat mouseLastX{}, mouseLastY{}, mouseChangeX{}, mouseChangeY{};
    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double positionX, double positionY);
};


#endif //OPENGLCOURSE_WINDOW_H
