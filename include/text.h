#ifndef TEXT_H
#define TEXT_H

#pragma once
#include <shader_m.h>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // size of glyph
    glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
    unsigned int Advance;   // horizontal offset to advance to next glyph
};

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer {
public:
    // holds a list of pre-compiled Characters
    std::map<char, Character> Characters;
    // shader used for text rendering
    // Shader TextShader = Shader("shaders/font_vs.glsl", "shaders/font_fs.glsl");
    // constructor
    TextRenderer();
    TextRenderer(unsigned int width, unsigned int height, Shader shader);
    // pre-compiles a list of characters from the given font
    void Load(std::string font, unsigned int fontSize);
    // renders a string of text using the precompiled list of characters
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

    void WriteText(std::ostringstream *text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

    unsigned int getFontSize();
    Shader getShader();
    void setShader(Shader);
    std::string getFont();
    void setFont(std::string);

private:
    // render state
    unsigned int fontSize;
    unsigned int VAO, VBO;
    Shader TextShader;
    std::string TextFont;
};

#endif