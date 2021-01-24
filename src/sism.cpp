#include "sism.h"
#include "filesys.h"
#include "glslcode.h"

namespace callback {
    structur::Bohr* instance = nullptr;

    void bindInstance(structur::Bohr* i) {
        instance = i;
    }

    structur::Bohr* getInstance(void) {
        return instance;
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
            glfwGetCursorPos(getInstance()->getWindow(), &x, &y);
        }
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if (getInstance()->getMouseData().first) {
            getInstance()->setMouseData(xpos, ypos, false);
            debugs("\n");
        }

        float xoffset = xpos - getInstance()->getMouseData().lastX;
        float yoffset = getInstance()->getMouseData().lastY - ypos;     // reversed since y-coordinates go from bottom to top
        getInstance()->setMouseData(xpos, ypos, false);
        getInstance()->getCamera().ProcessMouseMovement(xoffset, yoffset);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        getInstance()->getCamera().ProcessMouseScroll(yoffset);
    }
}

using namespace structur;

void structur::writeInstructions(TextRenderer tr, float x, float y, float scale) {
    tr.RenderText("          [O] Open File"      , x, y                                         , scale);
    tr.RenderText("       [WASD] Move camera"    , x, y + 1.f * scale * (tr.getFontSize() + 5.f), scale);
    tr.RenderText("[Arrows,2468] Rotate molecule", x, y + 2.f * scale * (tr.getFontSize() + 5.f), scale);
    tr.RenderText("      [Mouse] Rotate camera"  , x, y + 3.f * scale * (tr.getFontSize() + 5.f), scale);
    tr.RenderText("     [ESC, Q] Exit"           , x, y + 4.f * scale * (tr.getFontSize() + 5.f), scale);
}


void structur::writeAuthors(TextRenderer tr, float x, float y, float scale) {
    tr.RenderText("  Igor Nunes", x, y                                         , scale);
    tr.RenderText("Diogo Simoes", x, y + 1.f * scale * (tr.getFontSize() + 5.f), scale);
}


void structur::writeText(TextRenderer tr, string text, float x, float y, float scale) {
    tr.RenderText(text, x, y, scale);
}

const char* BohrException::what(void) const throw () { return message.c_str(); }

bool Bohr::initialize_glfw(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    this->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (this->window == NULL)
        return false;
    
    glfwMakeContextCurrent(this->window);
    
    this->setFrameBufferSizeCallback(callback::framebuffer_size_callback);
    this->setMouseButtonCallback(callback::mousebtn_callback);
    this->setCursorPositionCallback(callback::mouse_callback);
    this->setScrollCallback(callback::scroll_callback);

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    return true;
}

int Bohr::initialize_glad(void) {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

action Bohr::processInput(void) {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
    
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        this->camera.ProcessKeyboard(FORWARD, this->deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        this->camera.ProcessKeyboard(BACKWARD, this->deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        this->camera.ProcessKeyboard(LEFT, this->deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        this->camera.ProcessKeyboard(RIGHT, this->deltaTime);
    
    if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_8) == GLFW_PRESS)
        this->molrotx += 1.f;
    if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_2) == GLFW_PRESS)
        this->molrotx -= 1.f;
    if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_4) == GLFW_PRESS)
        this->molroty += 1.f;
    if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_6) == GLFW_PRESS)
        this->molroty -= 1.f;
    
    if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS)
        return action::CAMERA_RESET;
    
    if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS) {
        switchModeView(false);
        char *newfile = openPDBFileDialog();
        if (newfile != NULL) {
            if (isPDB(newfile)) {
                this->fname = string(newfile);
                free(newfile);
                switchModeView(true);
                return action::OPEN_FILE;
            } else {
                cout << "This is not a valid PDB file." << endl;
                osdialog_message(OSDIALOG_ERROR, OSDIALOG_OK, "This is not a valid PDB file.");
                switchModeView(true);
                free(newfile);
                return action::NO_ACTION;
            }
        }
        switchModeView(true);
        return action::NO_ACTION;
    }

    return action::NO_ACTION;
}

Camera &Bohr::getCamera(void) {
    return this->camera;
}

mouse_data Bohr::getMouseData(void) {
    return this->mouse;
}

void Bohr::setMouseData(float x, float y, bool first) {
    this->mouse = {x, y, first};
}

bool Bohr::getModeView(void) {
    return this->modeView;
}

void Bohr::switchModeView(bool mode) {
    this->modeView = mode;
    glfwSetInputMode(this->window, GLFW_CURSOR, mode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_HIDDEN);
}

char * Bohr::openPDBFileDialog(void) {
    return osdialog_file(OSDIALOG_OPEN, NULL, NULL, NULL);
}

bool Bohr::isPDB(char *fname) {
    return string(std::experimental::filesystem::path(fname).extension()).compare(".pdb") == 0;
}

void Bohr::setFrameBufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->window, callback);
}

void Bohr::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->window, callback);
}

void Bohr::setCursorPositionCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->window, callback);
}

void Bohr::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(this->window, callback);
}

bool Bohr::launchSuccessful(void) {
    return this->success;
}

string Bohr::getAppDir(void) {
    return this->appDir;
}

string Bohr::getAppName(void) {
    return this->appName;
}

string Bohr::getApp(void) {
    return this->appDir + "/" + this->appName;
}

string Bohr::getShaderDir(void) {
    return this->shaderDir;
}

string Bohr::getFontDir(void) {
    return this->fontDir;
}

string Bohr::getFont(void) {
    return this->fontDir + "/" + this->fontName;
}

GLFWwindow* Bohr::getWindow(void) {
    return this->window;
}

Shader Bohr::getMoleculeShader(void) {
    return this->shaderMolecule;
}

Shader Bohr::getFontShader(void) {
    return this->textRender.getShader();
}

TextRenderer Bohr::getTextRenderer(void) {
    return this->textRender;
}

void Bohr::refresh(void) {
    callback::bindInstance(this);

    glClearColor(0.f, 0.f, 51.f / 255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->currentFrame = glfwGetTime();
    this->deltaTime = this->currentFrame - this->lastFrame;
    this->lastFrame = this->currentFrame;

    if (this->logo.isAvailable() && this->fname.empty()) {
        this->logo.render(this->scr_width, this->scr_height);
    }

    writeInstructions(this->getTextRenderer(), 10.f, 10.f, 0.6f);
    writeAuthors(this->getTextRenderer(), 1470.f, this->scr_height - 2.f * this->getTextRenderer().getFontSize(), 0.75f);
    writeText(this->getTextRenderer(), !this->fname.empty() ? std::experimental::filesystem::path(this->fname).filename() : "No file opened", 10.f, this->scr_height - this->getTextRenderer().getFontSize(), 0.8f);

    switch (this->processInput()) {
        case action::OPEN_FILE:
            this->molecule.fromPDB(fname.data());
            break;
        
        case action::CAMERA_RESET:
            this->camera = this->molecule.resetCamera();
            this->molrotx = 0.f;
            this->molroty = 0.f;
            debugs("Camera position reseted.\n");
            break;

        default:
            break;
    }

    this->molecule.render_vanderWalls(this->shaderMolecule, this->camera, this->scr_width, this->scr_height, this->molrotx, this->molroty);

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}


void Bohr::terminate(void) {
    glfwTerminate();
}


void Bohr::main(void) {
    while (!glfwWindowShouldClose(this->getWindow())) {
        this->refresh();
    }
}

Bohr::Bohr(void) {}

Bohr::Bohr(const unsigned int width, const unsigned int height) {
    try {
        debugs("Lauching Bohr...\n");
        debugs("\tSetting relevant directories... ");
        std::experimental::filesystem::path appPath(filesys::getAppPath());
        this->appDir    = appPath.parent_path();
        this->appName   = appPath.filename();
        this->shaderDir = appDir + "/shaders";
        this->fontDir   = appDir + "/fonts";
        this->resDir    = appDir + "/res";
        this->fontName  = "UbuntuMono-R.ttf";
        this->logoName  = "bohr.bmp";

        debugs("[OK]\n\tInitializing GLFW and GLAD... ");
        this->scr_width   = width;
        this->scr_height  = height;
        this->setMouseData(this->scr_width / 2.0f, this->scr_height / 2.0f, true);
        if (!this->initialize_glfw(this->scr_width, this->scr_height, "BOHR - Very Small PDB Molecular Visualizer"))
            throw BohrException("Failed to create the window");
        if (!this->initialize_glad())
            throw BohrException("Failed to initialize GLAD");
        glEnable(GL_DEPTH_TEST);

        debugs("[OK]\n\tAuto-checking shaders... ");
        debugs("(corrected %d shaders) ", autoCorrectShaders(this->shaderDir));

        debugs("[OK]\n\tLoading shaders... ");
        this->shaderMolecule = Shader((this->shaderDir + "/" + MOLECULE_VS).c_str(), (this->shaderDir + "/" + MOLECULE_FS).c_str());
        if (!this->shaderMolecule.wasSuccessful())
            throw BohrException("Molecule: " + this->shaderMolecule.getReport());
        
        this->shaderFont     = Shader((this->shaderDir + "/" + FONT_VS).c_str(), (this->shaderDir + "/" + FONT_FS).c_str());
        if (!this->shaderFont.wasSuccessful())
            throw BohrException("Font: " + this->shaderFont.getReport());
        
        this->shaderLogo     = Shader((this->shaderDir + "/" + LOGO_VS).c_str(), (this->shaderDir + "/" + LOGO_FS).c_str());
        if (!this->shaderLogo.wasSuccessful())
            throw BohrException("Logo: " + this->shaderLogo.getReport());

        this->camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

        debugs("[OK]\n\tLoading text renderer... ");
        this->textRender = TextRenderer(this->scr_width, this->scr_height, this->shaderFont);
        this->textRender.Load(this->getFont(), 24);

        debugs("[OK]\n\tLoading logo... ");
        this->logo = Logo((this->resDir + "/" + this->logoName).c_str(), this->shaderLogo);
        // if (!this->logo.wasSuccessful())
        //     throw BohrException("Failed to load logo");

        debugs("[OK]\n");
        debugs("Done.\n\n");

        this->success = true;

    } catch (const BohrException &e) {
        debugs("[ERROR]\n");
        cout << "Error: " << e.what() << endl;
        cout << "Abort launch!" << endl;
        this->success = false;
    }
}

Bohr::~Bohr(void) {
    this->terminate();
}
