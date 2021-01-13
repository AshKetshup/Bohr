#include "../Headers/textRenderer.hpp"

namespace TextRenders {
    TextRenderer::TextRenderer(float width, float height) :
        shader("../../Asteroids/Shaders/text.vs", "../../Asteroids/Shaders/text.fs")
    {
        glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);

        shader.use();        
        shader.setMat4("projection", projection);

        // FreeType
        FT_Library ft;
        // Sempre que ocorre um erro, todas as funções devolvem um valor diferente de 0
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            
        // A fonte da letra é carregada como "face"
        FT_Face face;
        if (FT_New_Face(ft, "../../Asteroids/Fonts/Starjedi.ttf", 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // Aqui definimos o tamanho com que são carregados os glyphs 
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Aqui é desativada a restrição do alinhamento de bytes
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        // Aqui são carregados os primeiros 128 caracteres do set ASCII
        for (GLubyte c = 0; c < 128; c++)
        {
            // Carrega o caracter glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Aqui é gerada a textura
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // Aqui definimos as definições da textura
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Aqui armazenamos o caracter para usar mais tarde
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        
        // Assim que estiver terminado, o FreeType é destruído
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // Aqui são configurados os VAO/VBO para os quads da textura
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void TextRenderer::renderText(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // Aqui o estado de renderização correspondente a cada caracter é definido como ativo	
        shader.use();
        shader.setVec3("textColor", color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Aqui é repetido para todos os caracteres
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = characters[*c];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;

            // O VBO é atualizado para cada caracter
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
            // Renderizar a textura do quad acima do glyph
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            // O conteúdo da memória do VBO é atualizado
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            // Renderização do quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Aqui os cursores avançam para o próximo glyph com um avanço de 1/64 píxeis
            x += (ch.Advance >> 6) * scale; 
            // A linha anterior serve obter o valor em píxeis, fazendo um bitshift (2^6 = 64 (divide-se o número de 1/64 píxeis para obtermos a quantidade de píxeis)).
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
