#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

// distorts whole screen
vec4 Distort(vec2 texCoord)
{
	vec2 mid = vec2(0.5f);

	float distanceFromCentre = distance(texCoord, mid);
	vec2 normalisedCoord = normalize(texCoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;

	vec2 newCoord = mid + bias * normalisedCoord;
	return texture(colourTarget, newCoord);
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
	FragColour = Distort(texCoord);
}