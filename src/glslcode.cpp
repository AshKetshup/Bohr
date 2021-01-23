#include "glslcode.h"

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;
using namespace std;

string nameOfShader(int code) {
    if (code & (MOLECULE | VERTEX  ))  return string(MOLECULE_VS);
    if (code & (MOLECULE | FRAGMENT))  return string(MOLECULE_FS);
    if (code & (FONT     | VERTEX  ))  return string(FONT_VS);
    if (code & (FONT     | FRAGMENT))  return string(FONT_FS);
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
            
            default:
                shader.close();
                return false;
        }
        shader.close();
    }
    return true;
}

int autoCorrectShaders(string path) {
    int result = 0;
    result += (int) !createShader(path, MOLECULE | VERTEX  );
    result += (int) !createShader(path, MOLECULE | FRAGMENT);
    result += (int) !createShader(path, FONT     | VERTEX  );
    result += (int) !createShader(path, FONT     | FRAGMENT);
    return result;
}