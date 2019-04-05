#version 410

in vec2 vTexCoord;

uniform sampler2D renderTex;
uniform sampler2D depthTex;

// motion blur uniforms
uniform sampler2D texLinearDepth;
uniform mat4 inverseModelView;
uniform mat4 inverseProj;
uniform mat4 prevModelViewProj;

out vec4 FragColour;

int nSamples = 8;

vec3 GetViewRay()
{
	float x = vTexCoord.x * 2 - 1;
	float y = (1 - vTexCoord.y) * 2 - 1;
	float z = texture2D(depthTex, vTexCoord).z;
	vec4 projectedPos = vec4(x, y, z, 1);
	vec4 positionVS = projectedPos * inverseProj;
	return positionVS.xyz / positionVS.w;
}

// slight blur
vec4 MotionBlur(vec2 texCoord)
{
	// get current world space pos
	vec3 currentPos = GetViewRay() * texture(depthTex, vTexCoord).x;
	currentPos = currentPos * inverseModelView;

	// get previous screen space pos
	vec4 previousPos = prevModelViewProj * vec4(currentPos, 1);
	previousPos.xyz /= previousPos.w;
	previousPos.xy = previousPos.xy * 0.5 + 0.5;

	vec2 blurVec = previousPos.xy - vTexCoord;

	vec4 result = texture(renderTex, vTexCoord);
	for (int i = 1; i < nSamples; i++)
	{
		vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5);
		result += texture(renderTex, vTexCoord + offset);
	}
	return result /= float(nSamples);
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
	FragColour = MotionBlur(texCoord);
}