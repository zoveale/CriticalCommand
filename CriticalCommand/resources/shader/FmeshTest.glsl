#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;


void main()
{    
    FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);// + vec4(TexCoords.x, TexCoords.y, .5, 1.0);
}