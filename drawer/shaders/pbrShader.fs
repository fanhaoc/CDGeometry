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

//uniform vec3 albedo;
//uniform float metallic;
//uniform float roughness;
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform float ao;


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
// 菲涅尔项，反射和折射的比值, cosTheta是半程向量和摄像机方向的夹角
vec3 fresnelSchlick(float cosTheta, vec3 albedo, float metallic){
	vec3 F0 = vec3(0.04); //基础反射率
	F0 = mix(F0, albedo, metallic);
	return F0 + (1.0 -F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// 法线分布函数,微平面中法线与半程向量相同的比值
float distributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = 3.14159265359 * denom * denom;

	return num / denom;
}
// 几何函数
float geometrySchlickGGX(float NdotV, float roughness){
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // 粗糙度到k的直接映射
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return num / denom;
}
// 入射光和出射光都要受到几何函数的影响
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}

void main(){
	float shadow = shadowCalculation(v_pos, light.position);
	Material defaultMaterial = material;

	//贴图采样
	vec3 albedo = pow(texture(albedoMap, v_texture).rgb, vec3(2.2)); // 伽马矫正
	float metallic = texture(metallicMap, v_texture).r;
	float roughness = texture(roughnessMap, v_texture).r;
	
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(viewPos - v_pos);

	vec3 lightDir = normalize(light.position - v_pos);
	vec3 hDir = normalize(lightDir + viewDir);

	float distance = length(light.position - v_pos);
	float attenuation = 1 / (distance * distance);
	vec3 radiance = light.diffuse * attenuation;

	vec3 F = fresnelSchlick(max(dot(hDir, viewDir), 0.0), albedo, metallic);
	float NDF = distributionGGX(normal, hDir, roughness);
	float G = geometrySmith(normal, viewDir, lightDir, roughness);
	vec3 specular = (NDF * G * F)/(4.0 * max(dot(lightDir, normal), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001);

	vec3 KS = F;
	vec3 KD = vec3(1.0) - KS;
	KD = KD - metallic; // 金属不会折射光线

	float PI = 3.14159265359;
	float NdotL = max(dot(normal, lightDir), 0.0);
	
	vec3 Lo = (KD * albedo / PI + specular) * radiance * NdotL;
	vec3 ambient = light.ambient * albedo * ao;
	vec3 color = ambient + Lo;
	// gamma矫正和HDR线性映射，因为计算是在HDR空间做的
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));


	FragColor = vec4(color, 1.0);
}