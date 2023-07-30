//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_SHADER_H
#define OPENGLCOURSE_SHADER_H


#include <GL/glew.h>
#include <string>

#include "../light/directional-light.h"
#include "../shared/common-values.h"
#include "../light/point-light.h"

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
    void SetPointLight(PointLight *light, unsigned int lightCount);

private:
    int pointLightCount{};
    GLuint shaderId{}, uniformProjection{}, uniformView{}, uniformModel{}, uniformEyePosition{},
        uniformSpecularIntensity{}, uniformSpecularShininess{};

    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformDirection;
    } uniformDirectionalLight;

    GLuint uniformPointLightCount {};
    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint programId, const char* programCode, GLenum shaderType, const char* shaderName);
};


#endif //OPENGLCOURSE_SHADER_H
