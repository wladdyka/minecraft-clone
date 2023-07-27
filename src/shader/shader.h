//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_SHADER_H
#define OPENGLCOURSE_SHADER_H


#include <GL/glew.h>
#include <string>

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
    GLuint GetAmbientIntensityLocation();
    GLuint GetAmbientColourLocation();
    GLuint GetDiffuseIntensityLocation();
    GLuint GetDirectionLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();
    GLuint GetEyePositionLocation();

private:
    GLuint shaderId{}, uniformProjection{}, uniformView{}, uniformModel{}, uniformAmbientIntensity{},
        uniformAmbientColour{}, uniformDiffuseIntensity{}, uniformDirection{}, uniformEyePosition{},
        uniformSpecularIntensity{}, uniformSpecularShininess{};
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint programId, const char* programCode, GLenum shaderType, const char* shaderName);
};


#endif //OPENGLCOURSE_SHADER_H
