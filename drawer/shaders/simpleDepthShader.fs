#version 330 core

in vec4 FragPos;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Light light;
uniform float far_plane;

void main()
{             
    //gl_FragDepth = gl_FragCoord.z;
    float lightDistance = length(FragPos.xyz - light.position);
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}