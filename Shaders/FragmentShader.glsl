#version 330 

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main(){
	
	float ambientStrength = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularLight = 0.5;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectDir = reflect(-lightDirection, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	float specular = specularLight * spec;

	FragColor = (texture(tex0, texCoord) * (diffuse + ambientStrength) + texture(tex1, texCoord).r * specular) * lightColor;
}