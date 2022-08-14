#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 TexCoord;

uniform sampler2DArray texture0;

const vec3 lightDir = normalize(vec3(0.4, 0.3, 0.7));

void main()
{
	FragColor = texture(texture0, TexCoord);

	// Apply light
	FragColor = FragColor * (max(dot(Normal, lightDir), 0.0) * 0.5 + 0.5);
}