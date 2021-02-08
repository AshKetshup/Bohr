//------------------------------------------------------------------------------
//
// Bohr - Very Small PDB Molecular Visualizer
//
// RUNTIME LIBRARIES PACKAGE
//    glslcode.h
//
// DESCRIPTION:
// -----------
// Contains the shaders hard-coded in order to provide a failsafe option in case
// of the original files being missing.
// 
//------------------------------------------------------------------------------

#ifndef GLSLCODE_H
#define GLSLCODE_H

const char molecule_fragment_shader[] =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "in vec3 Normal;\n"
    "in vec3 FragPos;\n"
    "\n"
    "struct Lamp {\n"
    "    vec3 lightPos;\n"
    "    vec3 lightColor;\n"
    "    vec3 viewPos;\n"
    "};\n"
    "\n"
    "uniform vec3 objectColor;\n"
    "uniform Lamp lamp;\n"
    "\n"
    "void main() {\n"
    "    // ambient\n"
    "    float ambientStrength = 0.5;\n"
    "    vec3 ambient;\n"
    "    ambient = ambientStrength * lamp.lightColor;\n"
    "\n"
    "    // diffuse\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 lightDir;\n"
    "    float diff;\n"
    "    vec3 diffuse;\n"
    "    lightDir = normalize(lamp.lightPos - FragPos);\n"
    "    diff = max(dot(norm, lightDir), 0.0);\n"
    "    diffuse = diff * lamp.lightColor;\n"
    "\n"
    "    // specular\n"
    "    float specularStrength = 0.5; // this is set higher to better show the effect of Gouraud shading \n"
    "    vec3 viewDir = normalize(lamp.viewPos - FragPos);\n"
    "    vec3 reflectDir = reflect(-lightDir, norm);  \n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "    vec3 specular = specularStrength * spec * lamp.lightColor;\n"
    "\n"
    "    vec3 result = vec3(0.0);\n"
    "    result += (ambient + diffuse + specular);\n"
    "    result *= objectColor;\n"
    "\n"
    "    FragColor = vec4(result, 1.0);\n"
    "}";

const char molecule_vertex_shader[] =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;  \n"
    "    \n"
    "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}";

const char font_fragment_shader[] =
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "\n"
    "void main() {\n"
    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "    color = vec4(textColor, 1.0) * sampled;\n"
    "}";

const char font_vertex_shader[] =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "    TexCoords = vertex.zw;\n"
    "}";

const char logo_vertex_shader[] =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "uniform mat4 projection;\n"
    "\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}";

const char logo_fragment_shader[] =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "\n"
    "uniform sampler2D ourTexture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = texture(ourTexture, TexCoord);\n"
    "}";

#define VERTEX      0b00000001              // Vertex shader
#define FRAGMENT    0b00000010              // Fragment shader
#define GEOMETRY    0b00000100              // Geometry shader (for future use)
#define MOLECULE    0b00100000              // Molecule shaders
#define FONT        0b01000000              // Font shaders
#define LOGO        0b10000000              // Logo shaders

#define MOLECULE_FS "lighting_fs.glsl"      // Molecule fragment shader file name
#define MOLECULE_VS "lighting_vs.glsl"      // Molecule vertex shader file name
#define PISURF_FS   "pisurf_fs.glsl"        // Pi Surface fragment shader file name
#define PISURF_VS   "pisurf_vs.glsl"        // Pi Surface vertex shader file name
#define FONT_FS     "font_fs.glsl"          // Font fragment shader file name
#define FONT_VS     "font_vs.glsl"          // Font vertex shader file name
#define LOGO_FS     "logo_fs.glsl"          // Logo fragment shader file name
#define LOGO_VS     "logo_vs.glsl"          // Logo vertex shader file name

#include <string>

// Gets name of shader from the int code
std::string nameOfShader(int);

// Checks if a given shader exists
bool shaderExists(std::string);

// Creates the shader, if not available, given the int code
bool createShader(std::string, int);

// Proceeds to automatically check and create missing shaders.
// Returns the number of shaders that were missing and were successfully corrected.
int autoCorrectShaders(std::string);

#endif