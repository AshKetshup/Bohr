#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

struct Lamp {
    vec3 lightPos;
    vec3 lightColor;
    vec3 viewPos;
};

uniform vec3 objectColor;
uniform Lamp lamp;

void main() {
    // ambient
    float ambientStrength = 0.5;
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

    // specular
    float specularStrength = 0.5; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(lamp.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lamp.lightColor;

    vec3 result = vec3(0.0);
    result += (ambient + diffuse + specular);
    result *= objectColor;

    FragColor = vec4(result, 1.0);
}
