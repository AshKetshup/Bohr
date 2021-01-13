#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

struct Lamp {
    vec3 lightPos;
    vec3 lightColor;
};

uniform vec3 objectColor;
uniform Lamp lamp;

void main() {
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient;
    ambient = ambientStrength * lamp.lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir;
    float diff;
    vec3 diffuse;
    lightDir = normalize(lamp.lightPos - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = diff * lamp.lightColor;

    vec3 result = vec3(0.0);
    result += (ambient + diffuse);
    result *= objectColor;

    FragColor = vec4(result, 1.0);
}
