//
// Created by wladdyka on 7/23/2023.
//

#include <cstdio>
#include <cstring>
#include <fstream>

#include "shader.h"

void Shader::AddShader(GLuint programId, const char* programCode, GLenum shaderType, const char* shaderName) {
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

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("Error creating shader program");
        return;
    }

    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER, "vertex");
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER, "fragment");

    GLint linkingResult = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &linkingResult);
    if (!linkingResult) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking shader program: %s", errorLog);
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &linkingResult);
    if (!linkingResult) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error validating shader program: %s", errorLog);
    }

    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformView = glGetUniformLocation(shaderId, "view");
    uniformAmbientColour = glGetUniformLocation(shaderId, "directionalLight.colour");
    uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.ambientIntensity");
    uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.diffuseIntensity");
    uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
}

void Shader::CreateFromFile(const char* vertexCode, const char* fragmentCode) {
    CompileShader(ReadFile(vertexCode).c_str(), ReadFile(fragmentCode).c_str());
}

void Shader::UseShader() {
    glUseProgram(shaderId);

    if (useTransparentBlending) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
    }
}

void Shader::ClearShader() {
    if (shaderId != 0) {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader() {
    ClearShader();
}

GLuint Shader::GetProjectionLocation() {
    return uniformProjection;
}

GLuint Shader::GetViewLocation() {
    return uniformView;
}

GLuint Shader::GetModelLocation() {
    return uniformModel;
}

std::string Shader::ReadFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesn't exist!", filePath);
        return "";
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::GetAmbientIntensityLocation() {
    return uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColourLocation() {
    return uniformAmbientColour;
}

GLuint Shader::GetDiffuseIntensityLocation() {
    return uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation() {
    return uniformDirection;
}

Shader::Shader() = default;
