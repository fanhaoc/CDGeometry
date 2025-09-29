#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 model;
layout (std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texture;
void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	v_pos = vec3(model * vec4(aPos, 1.0));
	v_normal = mat3(transpose(inverse(model))) * aNormal;;
}