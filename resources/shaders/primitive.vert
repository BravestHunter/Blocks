#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 Color;

uniform mat4 MVP;
uniform vec4 FullColor;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0f);

	Color = FullColor;
}
