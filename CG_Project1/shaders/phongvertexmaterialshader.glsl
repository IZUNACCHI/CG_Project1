#version 330 core

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 VertPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = texCoord;
	VertPos = vec3(model * vec4(position, 1.0));
	Normal = normal;  
	gl_Position = projection * view * vec4(VertPos, 1.0);
};