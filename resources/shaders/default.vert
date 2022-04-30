#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 TexCoord;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0f);
	TexCoord = vec3(aTexCoord.x, aTexCoord.y, aTexCoord.z);
}