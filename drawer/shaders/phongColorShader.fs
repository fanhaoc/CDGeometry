out vec4 FragColor;

in vec3 v_pos;
in vec3 v_normal;
in vec4 v_lightSpacePos;

uniform vec3 viewPos;
uniform sampler2D shadowMap;


float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane  = 10.0;
    float z = depth * 2.0 - 1.0; // 转换为 NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
	// ndc坐标
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// 转换到[0,1]区间
	projCoords = projCoords * 0.5 + 0.5;

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float currentDepth = LinearizeDepth(projCoords.z);
	// pcf滤波
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > LinearizeDepth(pcfDepth) ? 0.0 : 1.0;        
		}    
	}
	shadow /= 9.0;

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

	float shadow = shadowCalculation(v_lightSpacePos, normal, lightDir);

	return (ambient + shadow * (diffuse + specular)) * attenuation;
	//return vec3(shadow, 0.0, 0.0);
}



void main(){
	Material defaultMaterial = material;
	vec3 normal = normalize(v_normal);
	vec3 resColor = c_phong(v_pos, normal, defaultMaterial);

	FragColor = vec4(resColor, 1.0);
}