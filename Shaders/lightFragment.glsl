#version 330

out vec4 FragColor;

uniform vec4 lightColor;

void main(){
	FragColor = lightColor;
}