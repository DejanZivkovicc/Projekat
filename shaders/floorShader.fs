#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
// uniform Light light;
uniform Light light1;   // Cube source of light
uniform Light light2;   // Pendulum source of light

vec3 CalculateLight(Light light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 texCoords) {
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return ambient + diffuse + specular;
}

void main() {
    // Normalization and calculations
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate contributions from both lights
    vec3 result1 = CalculateLight(light1, norm, viewDir, FragPos, TexCoords);
    vec3 result2 = CalculateLight(light2, norm, viewDir, FragPos, TexCoords);

    // Combine results from both lights
    vec3 result = result1 + result2;

    FragColor = vec4(result, 1.0);
}


//
// void main(){
//      // ambient
//      vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
//
//     // diffuse
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(light.position - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
//
//
//     // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
//
//     vec3 result = ambient + diffuse + specular; //* objectColor;
//     // FragColor = texture(floorTexture, TexCoords) * vec4(result, 1.0); //vec4(lightColor * objectColor, 1.0);
//     FragColor = vec4(result, 1.0);
// }