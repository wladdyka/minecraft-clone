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

private:
    GLint width, height;
    GLint bufferWidth{}, bufferHeight{};
    GLFWwindow* mainWindow{};
    bool keys[1024]{};
    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};


#endif //OPENGLCOURSE_WINDOW_H
