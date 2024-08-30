#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D floorTexture;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
    FragColor = texture(floorTexture, TexCoords); //vec4(lightColor * objectColor, 1.0);
}