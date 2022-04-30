#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

// texture samplers
//uniform sampler2D texture0;
uniform sampler2DArray texture0;

void main()
{
	FragColor = texture(texture0, TexCoord);
}