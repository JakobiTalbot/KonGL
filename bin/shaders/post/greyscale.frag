#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

vec4 Greyscale(vec2 texCoord)
{
	// get colour
	vec4 v4Colour = texture(colourTarget, texCoord);
	// find average value with weighted channels because human eyes are biased towards certain colours
	float averageRGB = 0.2126f * v4Colour.x + 0.7152f * v4Colour.y + 0.0722f * v4Colour.z;
	
	// return colour as greyscale with rgb values set to average
	return vec4(averageRGB, averageRGB, averageRGB, v4Colour.w);
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
	FragColour = Greyscale(texCoord);
}