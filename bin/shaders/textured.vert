// generic texture + lighting shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoord;

out vec4 vPosition;
out vec2 vTexCoord;
out vec3 vNormal;

uniform mat4 m4ProjectionViewModel;
uniform mat4 m4ModelMatrix;
uniform mat3 m3NormalMatrix;

void main()
{
	vPosition = m4ModelMatrix * Position;
	vNormal = m3NormalMatrix * Normal.xyz;
	vTexCoord = TexCoord;
	gl_Position = m4ProjectionViewModel * Position;
}