#version 330 core
out vec4 FragColor;

in vec3 v_pos;
in vec3 v_normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){
	vec3 lightPos = vec3(1.2, 1.0, 2.0);
	vec3 normal = normalize(v_normal);
	vec3 lightDir = normalize(lightPos - v_pos);
	vec3 viewDir = normalize(viewPos - v_pos);

	float ambientStrength = 0.3f;
	vec3 ambient = ambientStrength * lightColor;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfDir, normal), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 resultColor = (ambient + diffuse +specular) * objectColor;
	FragColor = vec4(resultColor, 1.0);
}