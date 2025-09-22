out vec4 FragColor;

in vec2 v_texcoord;
in vec3 v_pos;
in vec3 v_normal;

uniform vec3 viewPos;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

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

	return (ambient + diffuse + specular) * attenuation;
}

void main()
{
	Material defaultMaterail = material;
	defaultMaterail.diffuse =  texture(u_texture0, v_texcoord).rgb;
	defaultMaterail.ambient =  defaultMaterail.diffuse;
	defaultMaterail.specular = texture(u_texture1, v_texcoord).rgb;
	vec3 resColor = c_phong(v_pos, v_normal, defaultMaterail);

	vec3 emission = texture(u_texture2, v_texcoord).rgb;

    FragColor = vec4(resColor + emission, 1.0);
}