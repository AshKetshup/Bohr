#include "glslcode.h"

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;
using namespace std;

string nameOfShader(int code) {
    if (code & (MOLECULE | VERTEX  ))  return MOLECULE_VS;
    if (code & (MOLECULE | FRAGMENT))  return MOLECULE_FS;
    if (code & (FONT     | VERTEX  ))  return FONT_VS;
    if (code & (FONT     | FRAGMENT))  return FONT_FS;
    if (code & (LOGO     | VERTEX  ))  return LOGO_VS;
    if (code & (LOGO     | FRAGMENT))  return LOGO_FS;
}

bool shaderExists(string path) {
    return fs::exists(path);
}

bool createShader(string path, int code) {
    string fullpath = path + "/" + nameOfShader(code);
    if (!shaderExists(fullpath)) {
        ofstream shader(fullpath, ofstream::out);
        switch (code) {
            case MOLECULE | VERTEX:
                shader << molecule_vertex_shader;
                break;
            
            case MOLECULE | FRAGMENT:
                shader << molecule_fragment_shader;
                break;
            
            case FONT | VERTEX:
                shader << font_vertex_shader;
                break;
            
            case FONT | FRAGMENT:
                shader << font_fragment_shader;
                break;
            
            case LOGO | VERTEX:
                shader << logo_vertex_shader;
                break;
            
            case LOGO | FRAGMENT:
                shader << logo_fragment_shader;
                break;
            
            default:
                shader.close();
                return false;
        }
        shader.close();
    }
    return true;
}

int autoCorrectShaders(string path) {
    int c = 0;      // Number of corrected shaders
    c += (int) !createShader(path, MOLECULE | VERTEX  );
    c += (int) !createShader(path, MOLECULE | FRAGMENT);
    c += (int) !createShader(path, FONT     | VERTEX  );
    c += (int) !createShader(path, FONT     | FRAGMENT);
    c += (int) !createShader(path, LOGO     | VERTEX  );
    c += (int) !createShader(path, LOGO     | FRAGMENT);
    return c;
}