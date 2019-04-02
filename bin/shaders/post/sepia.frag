#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

vec4 Sepia(vec2 texCoord)
{
	// get colour
	vec4 v4Colour = texture(colourTarget, texCoord);

	// magic sepia numbers
	float r,g,b;
	r = (v4Colour.x * 0.393f) + (v4Colour.y * 0.769f) + (v4Colour.z * 0.189f);
	g = (v4Colour.x * 0.349f) + (v4Colour.y * 0.686f) + (v4Colour.z * 0.168f);
	b = (v4Colour.x * 0.272f) + (v4Colour.y * 0.534f) + (v4Colour.z * 0.131f);

	return vec4(r, g, b, v4Colour.w);
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
	FragColour = Sepia(texCoord);
}