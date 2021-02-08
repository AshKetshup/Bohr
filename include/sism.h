// SISM - Smart Interactive State Machine

#ifndef SISM_H
#define SISM_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader_m.h"
#include "camera.h"
#include "pdbreader.h"
#include "osdialog.h"
#include "text.h"
#include "logo.h"
#include "debug.h"

#include <string>
#include <experimental/filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace structur {
    using namespace std;

    typedef enum {
        NO_ACTION,
        CAMERA_RESET,
        OPEN_FILE
    } action;

    typedef enum {
        VANDERWALLS,
        PISURFACE
    } render_mode;

    typedef struct {
        float lastX;
        float lastY;
        bool  first;
    } mouse_data;

    void writeInstructions(TextRenderer, float, float, float);
    void writeAuthors(TextRenderer, float, float, float);
    void writeText(TextRenderer, string, float, float, float);

    class BohrException : public exception {
    public:
        BohrException(const string &msg) : message(msg) {}
        virtual const char* what() const throw ();
    private:
        const string message;
    };

    class Bohr {
    private:
        bool success = false;

        string appDir;
        string appName;

        string shaderDir;
        Shader shaderFont;
        Shader shaderMolecule;
        Shader shaderPiSurf;
        Shader shaderLogo;
        
        string fontDir;
        string fontName;
        TextRenderer textRender;

        string resDir;
        string logoName;
        Logo logo;

        render_mode rmode = VANDERWALLS;
        bool w_was_pressed = false;

        GLFWwindow* window;
        unsigned int scr_width;
        unsigned int scr_height;

        /* Timing */
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        float currentFrame;

        /* Molecule rotation */
        float molrotx = 0.f;
        float molroty = 0.f;

        /* Camera */
        Camera camera;
        mouse_data mouse; // = {0.f, 0.f, true};
        bool modeView = false;

        string fname;
        Molecule molecule;

        bool initialize_glfw(int, int, const char*);
        int initialize_glad(void);
        action processInput(void);

    public:
        Camera &getCamera(void);
        mouse_data getMouseData(void);
        void setMouseData(float, float, bool);
        bool getModeView(void);
        void switchModeView(bool);
        static char *openPDBFileDialog(void);
        static bool isPDB(char*);
        void setFrameBufferSizeCallback(GLFWframebuffersizefun);
        void setMouseButtonCallback(GLFWmousebuttonfun);
        void setCursorPositionCallback(GLFWcursorposfun);
        void setScrollCallback(GLFWscrollfun);
        bool launchSuccessful(void);
        string getAppDir(void);
        string getAppName(void);
        string getApp(void);
        string getShaderDir(void);
        string getFontDir(void);
        string getFont(void);
        GLFWwindow* getWindow(void);
        Shader getMoleculeShader(void);
        Shader getFontShader(void);
        TextRenderer getTextRenderer(void);
        void refresh(void);
        void terminate(void);
        void main(void);
        Bohr(void);
        Bohr(const unsigned int, const unsigned int);
        ~Bohr(void);
    };
}

#endif