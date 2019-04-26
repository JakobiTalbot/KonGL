#version 410

in vec2 vTexCoord;

uniform sampler2D renderTex;

out vec4 FragColour;

void main()
{
	// calculate texel size
	vec2 texSize = textureSize(renderTex, 0);
	vec2 texelSize = 1.0f / texSize;

	// adjust texture coordinate
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	// set frag colour
	FragColour = texture(renderTex, vTexCoord);
}