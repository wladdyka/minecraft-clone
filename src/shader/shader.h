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

    std::string ReadFile(const char* filePath);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();

private:
    GLuint shaderId{}, uniformProjection{}, uniformModel{};
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint programId, const char* programCode, GLenum shaderType, const char* shaderName);
};


#endif //OPENGLCOURSE_SHADER_H