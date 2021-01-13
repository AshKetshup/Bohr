//------------------------------------------------------------------------------
//
// UNIVERSIDADE DA BEIRA INTERIOR
// Licenciatura em Engenharia Informática, Computação Gráfica
// Projeto Prático: BOHR
//
//     41266 --- Diogo Castanheira Simões (diogo.c.simoes@ubi.pt)
//     41381 --- Igor Cordeiro Bordalo Nunes (igor.bordalo.nunes@ubi.pt)
//
//------------------------------------------------------------------------------


#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader_m.h"
#include "camera.h"
#include "pdbreader.hpp"
#include "osdialog.h"
#include "debug.h"
#include <string>
#include <map>

#include <experimental/filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

typedef enum {
    NO_ACTION,
    CAMERA_RESET,
    OPEN_FILE
} action;

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

GLFWwindow* initialize_glfw(int width, int height, const char* title);
int initialize_glad(void);
int initialize_fonts(void);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
action processInput(GLFWwindow *window, char **fname);

char *openPDBFileDialog(void);
bool isPBD(char *);

void switchModeView(GLFWwindow* window, bool mode);

void RenderText(Shader&, std::string, float, float, float, glm::vec3);


/* Settings */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Camera */
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool  firstMouse = true;
bool  modeView = false;

/* Molecule rotation */
float molrotx = 0.f;
float molroty = 0.f;

/* Timing */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/* Fonts */
unsigned int font_VAO, font_VBO;
std::map<char, Character> Characters;


int main(int argc, char const *argv[]) {
    GLFWwindow* window = initialize_glfw(SCR_WIDTH, SCR_HEIGHT, "BOHR - Very Small PDB Molecular Visualizer");
    if (window == NULL) {
        std::cout << "Failed to create the window" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!initialize_glad()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -2;
    }

    glEnable(GL_DEPTH_TEST);

    if (!initialize_fonts()) {
        glfwTerminate();
        std::cout << "Failed to initialize fonts" << std::endl;
        return -3;
    }

    debug("Loading shaders...\n");
    Shader lightingShader = Shader("shaders/lighting_vs.glsl", "shaders/lighting_fs.glsl");
    Shader fontShader = Shader("shaders/font_vs.glsl", "shaders/font_fs.glsl");

    char *fname = NULL;
    Molecule molecule;

    float currentFrame;
    while (!glfwWindowShouldClose(window)) {
        try {
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            RenderText(fontShader, "[O] Open file", 10.f, 10.f, 1.f, glm::vec3(1.f, 1.f, 1.f));

            switch (processInput(window, &fname)) {
                case action::OPEN_FILE:
                    if (fname != NULL) {
                        if (isPBD(fname)) {
                            cout << "Reading molecule from \"" << fname << "\"..." << endl;
                            molecule = Molecule().fromPDB(fname);
                            camera = molecule.resetCamera();
                            debug("\n%s\n", molecule.toString().data());
                        } else {
                            cout << "This is not a valid PDB file." << endl;
                            osdialog_message(OSDIALOG_ERROR, OSDIALOG_OK, "This is not a valid PDB file.");
                        }
                    } else {
                        cout << "No file was selected." << endl;
                        osdialog_message(OSDIALOG_INFO, OSDIALOG_OK, "No file was selected.");
                    }
                    if (fname != NULL) free(fname);
                    break;
                
                case action::CAMERA_RESET:
                    camera = molecule.resetCamera();
                    molrotx = 0.f;
                    molroty = 0.f;
                    cout << "Camera position reseted." << endl;
                    break;

                default:
                    break;
            }

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            molecule.render_vanderWalls(lightingShader, camera, SCR_WIDTH, SCR_HEIGHT, molrotx, molroty);

            glfwSwapBuffers(window);
            glfwPollEvents();
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        } catch (...) {
            std::cerr << "I dunno :(" << endl;
        }
    }

    glfwTerminate();
    
    return 0;
}


GLFWwindow* initialize_glfw(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
        return NULL;
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return window;
}


int initialize_glad(void) {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}


int initialize_fonts(void) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
        return 0;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/UbuntuMono-R.ttf", 0, &face)) {
        cout << "ERROR::FREETYPE: Failed to load font" << endl;
        return 0;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
            continue;
        }
        // generate texture
        unsigned int texture;
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
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &font_VAO);
    glGenBuffers(1, &font_VBO);
    glBindVertexArray(font_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 1;
}


void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color) {
    // activate corresponding render state	
    s.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    s.setMat4("projection", projection);
    s.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font_VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update font_VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of font_VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


action processInput(GLFWwindow *window, char **fname) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        molrotx += 1.f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        molrotx -= 1.f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        molroty += 1.f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        molroty -= 1.f;
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        return action::CAMERA_RESET;
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        switchModeView(window, false);
        *fname = openPDBFileDialog();
        switchModeView(window, true);
        return (*fname != NULL) ? action::OPEN_FILE : action::NO_ACTION;
    }

    return action::NO_ACTION;
}


char *openPDBFileDialog(void) {
    return osdialog_file(OSDIALOG_OPEN, NULL, NULL, NULL);
}


bool isPBD(char *fname) {
    return string(std::experimental::filesystem::path(fname).extension()).compare(".pdb") == 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}


void switchModeView(GLFWwindow* window, bool mode) {
    modeView = mode;
    glfwSetInputMode(window, GLFW_CURSOR, mode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_HIDDEN);
}