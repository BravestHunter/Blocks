#version 330 core
layout (location = 0) in uint aData;

uniform mat4 MVP;

out vec3 Normal;
out vec3 TexCoord;
out float AO;

const vec3 vertexOffsets[] = vec3[]
(
	vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), // front
	vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 1.0), vec3(0.0, 0.0, 1.0), // back
	vec3(1.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0), vec3(0.0, 1.0, 1.0), // right
	vec3(0.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 1.0), // left
	vec3(0.0, 1.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 0.0, 1.0), // top
	vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0)  // bottom
);

const vec3 normals[] = vec3[]
(
	vec3(1.0, 0.0, 0.0),
	vec3(-1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, -1.0, 0.0),
	vec3(0.0, 0.0, 1.0),
	vec3(0.0, 0.0, -1.0)
);

const vec3 coords[] = vec3[]
(
	vec3(0.0, 0.0, 0.0), // bottom left
	vec3(1.0, 0.0, 0.0), // bottom right
	vec3(0.0, 1.0, 0.0), // top left
	vec3(1.0, 1.0, 0.0)  // top right
);

const float AOs[] = float[] (0.4f, 0.6f, 0.8f, 1.0f);

void main()
{
	uint x = aData & 15u;
	uint y = (aData >> 4) & 15u;
	uint z = (aData >> 8) & 255u;
	vec3 position = vec3(float(x), float(y), float(z));

	uint face = (aData >> 16) & 7u;
	uint uv = (aData >> 19) & 3u;
	uint texture = (aData >> 21) & 511u;
	uint ao = (aData >> 30) & 3u;

	position = position + vertexOffsets[face * 4u + uv];

	gl_Position = MVP * vec4(position, 1.0);
	Normal = normals[face];
	TexCoord = coords[uv];
	TexCoord.z = float(texture);
	AO = AOs[ao];
}
