#version 410

in vec2 vTexCoord;

uniform sampler2D renderTex;

// chromatic aberration variables
uniform sampler2D noiseTexture;
uniform vec3 distortionOffsets = vec3(1, 0, -1);
uniform float distortionTime = 0;
uniform float distortionFrequency = 0.1;

out vec4 FragColour;

// slight blur
vec4 ChromaticAberration(vec2 texCoord)
{
	float distortion = 0.05*texture(noiseTexture, vec2( (vTexCoord.t + distortionTime) * distortionFrequency, 0.5f)).x;
	vec3 offsets = distortion * distortionOffsets;

	vec3 colour = vec3(texture(renderTex, vec2(vTexCoord.s + offsets.x, vTexCoord.t)).x,
					   texture(renderTex, vec2(vTexCoord.s + offsets.y, vTexCoord.t)).y,
					   texture(renderTex, vec2(vTexCoord.s + offsets.z, vTexCoord.t)).z);
	return vec4(colour, 1);
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
	FragColour = ChromaticAberration(texCoord);
}