#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define SHADER_VERTEX       0b00000001
#define SHADER_FRAGMENT     0b00000010
#define SHADER_GEOMETRY     0b00000100
#define SHADER_PROGRAM      0b00001000

#define TYPE_READING        0b00100000
#define TYPE_COMPILATION    0b01000000
#define TYPE_LINKING        0b10000000

class ShaderReport {
public:
    ShaderReport(void) : error(0), message("") {}
    ShaderReport(const int err, const std::string msg) : error(err), message(msg) {}
    void setReport(const int err, const std::string msg) { error = err; message = msg; }
    bool success(void) { return error == 0; }
    const std::string what() const throw () {
        std::string log("");
        if (error & TYPE_READING)       log += "Could not read ";
        if (error & TYPE_COMPILATION)   log += "Compilation on ";
        if (error & TYPE_LINKING)       log += "Linking on ";
        if (error & SHADER_VERTEX)      log += "vertex shader ";
        if (error & SHADER_FRAGMENT)    log += "fragment shader ";
        if (error & SHADER_GEOMETRY)    log += "geometry shader ";
        if (error & SHADER_PROGRAM)     log += "shader program ";
        log += "(message: " + message + ").";
        return log;
    }
private:
    int error;
    std::string message;
};


class Shader
{
public:
    unsigned int ID;
    Shader(void) { }
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        // 0. Define the path of the shader
        this->report         = ShaderReport();
        this->vertexShader   = std::string(vertexPath);
        this->fragmentShader = std::string(fragmentPath);
        this->geometryShader = std::string((geometryPath != nullptr) ? geometryPath : "");

        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vertexCode = vShaderStream.str();
        } catch (std::ifstream::failure& e) {
            report.setReport(TYPE_READING | SHADER_VERTEX, std::string(e.what()));
            return;
        }

        try {
            fShaderFile.open(fragmentPath);
            std::stringstream fShaderStream;
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            report.setReport(TYPE_READING | SHADER_FRAGMENT, std::string(e.what()));
            return;
        }

        if (geometryPath != nullptr) {
            try {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            } catch (std::ifstream::failure& e) {
                report.setReport(TYPE_READING | SHADER_GEOMETRY, std::string(e.what()));
                return;
            }
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        if (!checkCompileErrors(vertex, SHADER_VERTEX)) return;
        
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if (!checkCompileErrors(fragment, SHADER_FRAGMENT)) return;
        
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if(geometryPath != nullptr)
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            if (!checkCompileErrors(geometry, SHADER_GEOMETRY)) return;
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if(geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        if (!checkCompileErrors(ID, SHADER_PROGRAM)) return;
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath != nullptr)
            glDeleteShader(geometry);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    std::string getVertexShaderPath(void) {
        return this->vertexShader;
    }

    std::string getFragmentShaderPath(void) {
        return this->fragmentShader;
    }

    std::string getGeometryShaderPath(void) {
        return this->geometryShader;
    }

    bool wasSuccessful(void) {
        return this->report.success();
    }

    std::string getReport(void) {
        return this->report.what();
    }

    ShaderReport getReportHandler(void) {
        return this->report;
    }

private:
    std::string vertexShader;
    std::string fragmentShader;
    std::string geometryShader;
    ShaderReport report;

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    bool checkCompileErrors(GLuint shader, const int type) {
        GLint success;
        GLchar infoLog[1024];
        if (!(type & SHADER_PROGRAM)) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                report.setReport(TYPE_COMPILATION | type, std::string(infoLog));
                return false;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                report.setReport(TYPE_LINKING | type, std::string(infoLog));
                return false;
            }
        }
        return true;
    }
};
#endif
