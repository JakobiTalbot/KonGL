#version 410

in vec2 vTexCoord;

uniform sampler2D renderTex;

// swirl effect parameters
uniform float radius = 400.0f;
uniform float angle = 0.7f;
uniform vec2 centre = vec2(640, 360);
uniform int width = 1280;
uniform int height = 720;

out vec4 FragColour;

vec4 Swirl(vec2 texCoord)
{
	vec2 texSize = vec2(width, height);
	vec2 tc = texCoord * texSize;
	tc -= centre;
	float dist = length(tc);
	
	if (dist < radius)
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0f;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
	}
	tc += centre;
	vec3 col = texture(renderTex, tc / texSize).rgb;
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
	FragColour = Swirl(texCoord);
}