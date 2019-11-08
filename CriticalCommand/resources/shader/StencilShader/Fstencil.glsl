#version 460
out vec4 FragColor;

in vec3 normal;
in vec3 colorOut;

void main(){
	
	//FIXME:: change once lamp models are right direction
    //FragColor = vec4(normal.rgb, 1.0) + vec4(0.3);
	//vec3 color = vec3(1.0);
	//FragColor = vec4(vec3(0.0627f, 0.902f, 0.0314f), 1.0f);
	FragColor = vec4(colorOut, 1.0f);
}