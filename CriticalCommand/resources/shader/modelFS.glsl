#version 330 core
out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D texture_diffuse1;


void main()
{    
    FragColor = vec4(texture(texture_diffuse1, textureCoords).rgb, 1.0) + vec4(textureCoords.x, textureCoords.y, .5, 1.0);
}