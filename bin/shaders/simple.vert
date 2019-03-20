#version 410

layout( location = 0) in vec4 Position;

uniform mat4 m4ProjectionViewModel;

void main()
{
	gl_Position = m4ProjectionViewModel * Position;
}