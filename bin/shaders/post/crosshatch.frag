#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

// crosshatch effect parameters
uniform float offset = 2.0f;
uniform float hatch_y_offset = 2.0f;
uniform float lum_threshold_1 = 1.0f;
uniform float lum_threshold_2 = 0.7f;
uniform float lum_threshold_3 = 0.5f;
uniform float lum_threshold_4 = 0.3f;

out vec4 FragColour;

vec4 Crosshatch(vec2 texCoord)
{
	vec3 tc = vec3(1, 0, 0);

	// shouldn't use this many if statements in a shader
	if (texCoord.x < offset - 0.005f)
	{
		float lum = length(texture(colourTarget, texCoord).xyz);
		tc = vec3(1);

		if (lum < lum_threshold_1 && mod(gl_FragCoord.x + gl_FragCoord.y, 10.0f) == 0.0f)
			tc = vec3(0);
		if (lum < lum_threshold_2 && mod(gl_FragCoord.x - gl_FragCoord.y, 10.0f) == 0.0f)
			tc = vec3(0);
		if (lum < lum_threshold_3 && mod(gl_FragCoord.x + gl_FragCoord.y - hatch_y_offset, 10.0f) == 0.0f)
			tc = vec3(0);
		if (lum < lum_threshold_4 && mod(gl_FragCoord.x - gl_FragCoord.y - hatch_y_offset, 10.0f) == 0.0f)
			tc = vec3(0);
	}
	else if (texCoord.x >= offset + 0.005f)
		tc = texture(colourTarget, texCoord).xyz;

	return vec4(tc, 1);
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
	FragColour = Crosshatch(texCoord);
}