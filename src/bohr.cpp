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
#include "debug.h"

using namespace std;

GLFWwindow* initialize_glfw(int width, int height, const char* title);
int initialize_glad(void);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


/* Settings */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Camera */
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool  firstMouse = true;

/* Timing */
float deltaTime = 0.0f;
float lastFrame = 0.0f;


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
    // Shader lampShader = Shader("shaders/lamp_vs.glsl", "shaders/lamp_fs.glsl");

    debug("Reading molecule...\n");
    Molecule molecule = Molecule().fromPDB("pdb/gabj-chainA-model.pdb");
    cout << molecule.toString() << endl;

    camera = molecule.resetCamera();

    float currentFrame;
    while (!glfwWindowShouldClose(window)) {
        try {
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            debug("Rendering at %.3f\n", currentFrame);
            molecule.render_vanderWalls(lightingShader, camera, SCR_WIDTH, SCR_HEIGHT);

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
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}


int initialize_glad(void) {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
