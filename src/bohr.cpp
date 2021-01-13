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

#include <experimental/filesystem>

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
void mousebtn_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
action processInput(GLFWwindow *window, char **fname);

char *openPDBFileDialog(void);
bool isPBD(char *);

void switchModeView(GLFWwindow* window, bool mode);


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

// Camera position
float x = 0.0, y = -5.0;  // initially 5 units south of origin
float deltaMove = 0.0;    // initially camera doesn't move

// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0;        // angle of rotation for the camera direction
float deltaAngle = 0.0;   // additional angle change when dragging

// Mouse drag control
int isDragging = 0;       // true when dragging
int xDragStart = 0;       // records the x-coordinate when dragging starts

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

    char *fname = NULL;
    Molecule molecule;

    float currentFrame;
    while (!glfwWindowShouldClose(window)) {
        try {
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            switch (processInput(window, &fname)) {
                case action::OPEN_FILE:
                    if (fname != NULL) {
                        if (isPBD(fname)) {
                            cout << "Reading molecule from \"" << fname << "\"..." << endl;
                            molecule = Molecule().fromPDB(fname);
                            camera = molecule.resetCamera();
                            debug("%s\n", molecule.toString().data());
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
    
    // process mouse button push/release
    glfwSetMouseButtonCallback(window, mousebtn_callback);

    // process mouse dragging motion
    glfwSetCursorPosCallback(window, mouse_callback);

    // process mouse scrolling motion
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return window;
}


int initialize_glad(void) {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        // debug("Up\n");
        molrotx += 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        // debug("Down\n");
        molrotx -= 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        // debug("Left\n");
        molroty += 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        // debug("Right\n");
        molroty -= 1.f;
    }
    
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

void mousebtn_callback(GLFWwindow *window, int button, int action, int mods) {
    bool lbutton_down;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (GLFW_PRESS == action)
            lbutton_down = true;
        else if (GLFW_RELEASE == action)
            lbutton_down = false;
    }

    if (lbutton_down) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

    }
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