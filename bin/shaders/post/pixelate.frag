#version 410

in vec2 vTexCoord;

uniform sampler2D renderTex;
uniform int pixels = 2048;
out vec4 FragColour;

vec4 Pixelate(vec2 texCoord)
{
	float dx = 15.0f * (1.0f / pixels);
	float dy = 10.0f * (1.0f / pixels);
	vec2 v2Coord = vec2(dx * floor(texCoord.x / dx), dy * floor(texCoord.y / dy));
	return texture(renderTex, v2Coord);
}

void main()
{
	// calculate texel size
	vec2 texSize = textureSize(renderTex, 0);
	vec2 texelSize = 1.0f / texSize;

	// adjust texture coordinate
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	// set frag colour
	FragColour = Pixelate(texCoord);
}