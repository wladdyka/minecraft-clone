//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_TEXT_H
#define OPENGLCOURSE_TEXT_H


#include <map>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/detail/type_vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text {
public:
    Text();
    Text(unsigned int width, unsigned int height);
    ~Text();
    void RenderText(GLuint shaderId, std::string text, float x, float y, float scale, glm::vec3 color);
private:
    void Init();
    unsigned int screenWidth = 800;
    unsigned int screenHeight = 600;
    std::map<GLchar, Character> Characters;
    unsigned int vaoId, vboId;
};


#endif //OPENGLCOURSE_TEXT_H
