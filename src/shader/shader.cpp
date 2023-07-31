//
// Created by wladdyka on 7/23/2023.
//

#include <cstdio>
#include <cstring>
#include <fstream>

#include "shader.h"
#include "../shared/common-values.h"

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
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderId, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
    uniformSpecularShininess = glGetUniformLocation(shaderId, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].base.colour", i);
        uniformPointLight[i].uniformColour = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zd].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightCount");
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.base.colour", i);
        uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zd].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderId, locBuff);
    }
}

void Shader::CreateFromFile(const char* vertexCode, const char* fragmentCode) {
    CompileShader(ReadFile(vertexCode).c_str(), ReadFile(fragmentCode).c_str());
}

void Shader::UseShader() {
    glUseProgram(shaderId);

    if (useTransparentBlending) {
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
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

GLuint Shader::GetSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation() {
    return uniformSpecularShininess;
}

GLuint Shader::GetEyePositionLocation() {
    return uniformEyePosition;
}

void Shader::SetDirectionalLight(DirectionalLight *light) {
    light->UseLight(
            uniformDirectionalLight.uniformAmbientIntensity,
            uniformDirectionalLight.uniformColour,
            uniformDirectionalLight.uniformDiffuseIntensity,
            uniformDirectionalLight.uniformDirection
    );
}

void Shader::SetPointLights(PointLight *light, unsigned int lightCount) {
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i =0; i < lightCount; i++) {
        light[i].UseLight(
                uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
                uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent
        );
    }
}

void Shader::SetSpotLights(SpotLight *light, unsigned int lightCount) {
    if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        light[i].UseLight(
                uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
                uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
                uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformConstant,
                uniformSpotLight[i].uniformEdge
        );
    }
}

Shader::Shader() = default;
