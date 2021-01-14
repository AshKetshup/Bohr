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
#include "text.h"
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

GLFWwindow* initialize_glfw(int width, int height, const char* title);
int initialize_glad(void);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
action processInput(GLFWwindow *window, char **fname);

char *openPDBFileDialog(void);
bool isPBD(char *);

void switchModeView(GLFWwindow* window, bool mode);
void writeInstructions(TextRenderer, float, float, float);
void writeText(TextRenderer, string, float, float, float);


/* Settings */
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

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

    debug("Loading shaders...\n");
    Shader lightingShader = Shader("shaders/lighting_vs.glsl", "shaders/lighting_fs.glsl");
    // Shader fontShader = Shader("shaders/font_vs.glsl", "shaders/font_fs.glsl");

    TextRenderer textrenderer = TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    try {
        textrenderer.Load("fonts/UbuntuMono-R.ttf", 24);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    char *fname = NULL;
    Molecule molecule;

    float currentFrame;
    while (!glfwWindowShouldClose(window)) {
        try {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            writeInstructions(textrenderer, 10.f, 10.f, 0.6f);
            writeText(textrenderer, (fname != NULL) ? std::experimental::filesystem::path(fname).filename() : "No file opened", 10.f, SCR_HEIGHT - textrenderer.getFontSize(), 0.8f);

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
                    // if (fname != NULL) free(fname);
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

            molecule.render_vanderWalls(lightingShader, camera, SCR_WIDTH, SCR_HEIGHT, molrotx, molroty);

            glfwSwapBuffers(window);
            glfwPollEvents();
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        } catch (...) {
            std::cerr << "I dunno :(" << endl;
        }
    }

    if (fname != NULL) free(fname);

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


void writeInstructions(TextRenderer tr, float x, float y, float scale) {
    tr.RenderText("          [O] Open File"      , x, y                                         , scale);
    tr.RenderText("       [WASD] Move camera"    , x, y + 1.f * scale * (tr.getFontSize() + 5.f), scale);
    tr.RenderText("[Arrows,2468] Rotate molecule", x, y + 2.f * scale * (tr.getFontSize() + 5.f), scale);
    tr.RenderText("      [Mouse] Rotate camera"  , x, y + 3.f * scale * (tr.getFontSize() + 5.f), scale);
    tr.RenderText("     [ESC, Q] Exit"           , x, y + 4.f * scale * (tr.getFontSize() + 5.f), scale);
}


void writeText(TextRenderer tr, string text, float x, float y, float scale) {
    tr.RenderText(text, x, y, scale);
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
        if (*fname != NULL) free(*fname);
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