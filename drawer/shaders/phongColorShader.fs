out vec4 FragColor;

in vec3 v_pos;
in vec3 v_normal;

uniform vec3 viewPos;

vec3 c_phong(vec3 fragPos, vec3 normal){
	// 环境光
	vec3 ambient = material.ambient * light.ambient;
	// 漫反射
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * material.diffuse * light.diffuse;
	// 镜面反射
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular * light.specular;

	return ambient + diffuse + specular;
}

void main(){
	vec3 normal = normalize(v_normal);
	vec3 resColor = c_phong(v_pos, normal);

	FragColor = vec4(resColor, 1.0);
}