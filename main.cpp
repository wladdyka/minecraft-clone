#include <cstdio>
#include <cstring>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 1920, HEIGHT = 1080;
const float toRadians = 3.14159265f / 180.0f;
GLuint vaoId, vboId, shaderId, uniformModelId;

bool directionRight = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleOffsetIncrement = 0.0005f;

static const char* vertexShaderCode = "#version 330\n"
                                      "layout (location = 0) in vec3 pos;\n"
                                      "uniform mat4 model;\n"
                                      "void main() {\n"
                                      "gl_Position = model * vec4(0.2 * pos.x, 0.2 * pos.y, pos.z, 1.0);\n"
                                      "}";

static const char* fragmentShaderCode = "#version 330\n"
                                        "out vec4 colour;\n"
                                        "void main() {\n"
                                        "colour = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                        "}";

void AddShader(GLuint programId, const char* programCode, GLenum shaderType, const char* shaderName) {
    GLuint shaderToAdd = glCreateShader(shaderType);

    const GLchar* shaderCode[1];
    shaderCode[0] = programCode;

    GLint codeLength[1];
    codeLength[0] = strlen(programCode);

    glShaderSource(shaderToAdd, 1, shaderCode, codeLength);
    glCompileShader(shaderToAdd);

    GLint compilationResult = 0;
    GLchar errorLog[1024] = { 0 };

    glGetShaderiv(shaderToAdd, GL_COMPILE_STATUS, &compilationResult);
    if (!compilationResult) {
        glGetShaderInfoLog(shaderToAdd, sizeof(errorLog), nullptr, errorLog);
        printf("Error compiling shader %s program: %s", shaderName, errorLog);
        return;
    }

    glAttachShader(programId, shaderToAdd);
}

void CompileShaders() {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("Error creating shader program");
        return;
    }

    AddShader(shaderId, vertexShaderCode, GL_VERTEX_SHADER, "vertex");
    AddShader(shaderId, fragmentShaderCode, GL_FRAGMENT_SHADER, "fragment");

    GLint linkingResult = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &linkingResult);
    if (!linkingResult) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking shader program: %s", errorLog);
        return;
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &linkingResult);
    if (!linkingResult) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error validating shader program: %s", errorLog);
        return;
    }

    uniformModelId = glGetUniformLocation(shaderId, "model");
}

void CreateTriangle () {
    GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    while(!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        if (directionRight) {
            triangleOffset += triangleOffsetIncrement;
        } else {
            triangleOffset -= triangleOffsetIncrement;
        }

        if (std::abs(triangleOffset) >= triangleMaxOffset) {
            directionRight = !directionRight;
        }

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderId);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(triangleOffset, 0.0f, 0.0f));

        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vaoId);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
