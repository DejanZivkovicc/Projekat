#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sphereTexture;
uniform vec3 sphereColor;

void main(){
//     FragColor = vec4(0.3f, 0.2f, 0.5f, 1.0f);
    FragColor = texture(sphereTexture, TexCoords) * vec4(sphereColor, 1.0f); //* vec4(1.0f, 0.0f, 0.0f, 1.0f);
}