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

#define VERTEX      0b00000001
#define FRAGMENT    0b00000010
#define GEOMETRY    0b00000100     // for future use
#define MOLECULE    0b01000000
#define FONT        0b10000000

#define MOLECULE_FS "lighting_fs.glsl"
#define MOLECULE_VS "lighting_vs.glsl"
#define FONT_FS     "font_fs.glsl"
#define FONT_VS     "font_vs.glsl"

#include <string>

std::string nameOfShader(int);
bool shaderExists(std::string);
bool createShader(std::string, int);
int autoCorrectShaders(std::string);

#endif