#version 410

in vec2 vTexCoord;

uniform sampler2D renderTex;

out vec4 FragColour;

vec4 EdgeDetection(vec2 texCoord)
{
	const float offset = 1.0f / 300.0f;

	// declare offsets
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	// declare kernel
	float kernel[9] = float[]
	(
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(renderTex, texCoord.st + offsets[i]));
	}
	vec3 col = vec3(0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0f);
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
	FragColour = EdgeDetection(texCoord);
}