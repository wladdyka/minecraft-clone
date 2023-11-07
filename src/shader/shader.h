//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_SHADER_H
#define OPENGLCOURSE_SHADER_H


#include <GL/glew.h>
#include <string>

#include "../light/directional-light.h"
#include "../common/common-values.h"

class Shader {
public:
    Shader();
    ~Shader();

    void CreateFromFile(const char* vertexCode, const char* fragmentCode);
    void UseShader();
    void ClearShader();
    bool useTransparentBlending = false;

    std::string ReadFile(const char* filePath);

    GLuint GetId() { return shaderId; };
    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();
    GLuint GetEyePositionLocation();

    void SetDirectionalLight(DirectionalLight *light);

private:
    GLuint shaderId{}, uniformProjection{}, uniformView{}, uniformModel{}, uniformEyePosition{},
        uniformSpecularIntensity{}, uniformSpecularShininess{};

    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformDirection;
    } uniformDirectionalLight;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint programId, const char* programCode, GLenum shaderType, const char* shaderName);
};


#endif //OPENGLCOURSE_SHADER_H
