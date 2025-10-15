#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texture;

uniform vec3 viewPos;
uniform samplerCube shadowMap;
uniform float far_plane;


float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane  = 10.0;
    float z = depth * 2.0 - 1.0; // 转换为 NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

float shadowCalculation(vec3 fragPos, vec3 lightPos){
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(shadowMap, fragToLight).r;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.15;
	// percentage-closer filtering
	float shadow= 0.0;
	int samplerNum = 20;
	float viewDist = length(viewPos - fragPos);
	float diskRadius = (1.0 +viewDist / far_plane)/25.0;
	vec3 sampleOffsetDirections[20] = vec3[]
	(
	   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);
	for(int i = 0; i != samplerNum; ++i)
	{
		float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;   // Undo mapping [0;1]
		if(currentDepth - bias < closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samplerNum);
	return shadow;
}

vec3 c_phong(vec3 fragPos, vec3 normal, Material material){
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

	float distanceFL = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distanceFL + light.quadratic * distanceFL * distanceFL);

	float shadow = shadowCalculation(v_pos, light.position);

	return (ambient + shadow * (diffuse + specular)) * attenuation;
	//return vec3(shadow, 0.0, 0.0);
}



void main(){
	Material defaultMaterial = material;
	vec3 normal = normalize(v_normal);
	vec3 resColor = c_phong(v_pos, normal, defaultMaterial);

	FragColor = vec4(resColor, 1.0);
}