#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

vec4 Pixelate(vec2 texCoord)
{
	float fPixels = 2048.0f;
	float dx = 15.0f * (1.0f / fPixels);
	float dy = 10.0f * (1.0f / fPixels);
	vec2 v2Coord = vec2(dx * floor(texCoord.x / dx), dy * floor(texCoord.y / dy));
	return texture(colourTarget, v2Coord);
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
	FragColour = Pixelate(texCoord);
}