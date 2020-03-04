#version 460
out vec4 FragColor;

in vec3 normal;
uniform vec3 lampColor;
void main(){
	
	//FIXME:: change once lamp models are right direction
    //FragColor = vec4(normal.rgb, 1.0) + vec4(0.3);
	//vec3 color = vec3(1.0);
	FragColor = vec4(lampColor, 1.0);
}