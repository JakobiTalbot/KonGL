#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

// DOF variables
uniform sampler2D depthImg;
uniform float depthModifier = 1.0f;

out vec4 FragColour;

float LineariseDepth(float zoverw)
{
	float n = 0.1f; // camera z near
	float f = 2000.0f; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

// slight blur
vec4 DOF(vec2 texCoord)
{
	float depth = texture(depthImg, vTexCoord.xy).x;
	depth = LineariseDepth(depth) * depthModifier;
	
	return vec4(depth, depth, depth, 1);
}

void main()
{
	// calculate texel size
	vec2 texSize = textureSize(colourTarget, 0);
	vec2 texelSize = 1.0f / texSize;

	// adjust texture coordinate
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	// set frag colour
	FragColour = DOF(texCoord);
}