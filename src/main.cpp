#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh/mesh.h"
#include "shader/shader.h"

const GLint WIDTH = 1920, HEIGHT = 1080;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

bool directionRight = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleOffsetIncrement = 0.0005f;
float currentAngle = 0.0f;

void CreateShader() {
    auto* shader1 = new Shader();
    shader1->CreateFromFile("shaders/shader.vertex", "shaders/shader.fragment");
    shaderList.push_back(shader1);
}

void CreateObjects() {
    unsigned int indices[] = {
           0, 3, 1,
           1, 3, 2,
           2, 3, 0,
           0, 1, 2
    };

    GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    Mesh *object1 = new Mesh();
    object1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(object1);

    Mesh *object2 = new Mesh();
    object2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(object2);
}

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
    if (!mainWindow) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateObjects();
    CreateShader();

    GLuint uniformModelId, uniformProjectionId;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        if (directionRight) {
            triangleOffset += triangleOffsetIncrement;
        } else {
            triangleOffset -= triangleOffsetIncrement;
        }

        currentAngle += 0.01f;
        if (currentAngle >= 360) {
            currentAngle -= 360;
        }

        if (std::abs(triangleOffset) >= triangleMaxOffset) {
            directionRight = !directionRight;
        }

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModelId = shaderList[0]->GetModelLocation();
        uniformProjectionId = shaderList[0]->GetProjectionLocation();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjectionId, 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 2.0f, -5.0f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
