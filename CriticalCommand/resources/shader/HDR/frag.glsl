#version 460 core
out vec4 FragColor;
  
in vec2 textureUV;

uniform sampler2D hdrBuffer;

void main(){             
    vec3 hdrColor = texture(hdrBuffer, textureUV).rgb;
    FragColor = vec4(hdrColor, 1.0);
}  