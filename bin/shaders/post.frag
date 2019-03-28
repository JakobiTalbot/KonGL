#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform float width;
uniform float height;

// swirl effect parameters
uniform float radius = 400.0f;
uniform float angle = 0.7f;
uniform vec2 centre = vec2(0.0f, 0.0f);

// crosshatch effect parameters
uniform float offset;
uniform float hatch_y_offset;
uniform float lum_threshold_1 = 1.0f;
uniform float lum_threshold_2 = 0.7f;
uniform float lum_threshold_3 = 0.5f;
uniform float lum_threshold_4 = 0.3f;

out vec4 FragColour;

// output colour unchanged
vec4 Default(vec2 texCoord)
{
	return texture(colourTarget, texCoord);
}

// slight blur
vec4 Blur(vec2 texCoord)
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
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f,  4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(colourTarget, texCoord.st + offsets[i]));
	}
	vec3 col = vec3(0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0f);
}

vec4 Crosshatch(vec2 texCoord, bool invert)
{
	vec3 tc = vec3(1, 0, 0);

	if (texCoord.x < offset - 0.005f)
	{
		float lum = length(texture(colourTarget, texCoord).xyz);
		tc = vec3(1);

		if (lum < lum_threshold_1 && mod(gl_FragCoord.x + gl_FragCoord.y, 10.0f) == 0.0f)
			tc = vec3(0);
		else if (lum < lum_threshold_2 && mod(gl_FragCoord.x - gl_FragCoord.y, 10.0f) == 0.0f)
			tc = vec3(0);
		else if (lum < lum_threshold_3 && mod(gl_FragCoord.x + gl_FragCoord.y - hatch_y_offset, 10.0f) == 0.0f)
			tc = vec3(0);
		else if (lum < lum_threshold_4 && mod(gl_FragCoord.x - gl_FragCoord.y - hatch_y_offset, 10.0f) == 0.0f)
			tc = vec3(0);
	}
	else if (texCoord.x >= offset + 0.005f)
		tc = texture(colourTarget, texCoord).xyz;

	vec4 test = vec4(tc, 1);

	if (invert)
		return vec4(vec3(1.0f - test), 1.0f);
	else
		return test;
}

// distorts to centre of screen
vec4 Distort(vec2 texCoord)
{
	vec2 mid = vec2(0.5f);

	float distanceFromCentre = distance(texCoord, mid);
	vec2 normalisedCoord = normalize(texCoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;

	vec2 newCoord = mid + bias * normalisedCoord;
	return texture(colourTarget, newCoord);
}

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
		sampleTex[i] = vec3(texture(colourTarget, texCoord.st + offsets[i]));
	}
	vec3 col = vec3(0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0f);
}

vec4 Emboss(vec2 texCoord)
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
		-2, -1, 0,
		-1, 1, 1,
		0, 1, 2
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(colourTarget, texCoord.st + offsets[i]));
	}
	vec3 col = vec3(0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0f);
}

vec4 Greyscale(vec2 texCoord)
{
	// get colour
	vec4 v4Colour = texture(colourTarget, texCoord);
	// find average value with weighted channels because of science
	float averageRGB = 0.2126f * v4Colour.x + 0.7152f * v4Colour.y + 0.0722f * v4Colour.z;
	
	// return colour as greyscale with rgb values set to average
	return vec4(averageRGB, averageRGB, averageRGB, v4Colour.w);
}

vec4 Invert(vec2 texCoord)
{
	return vec4(vec3(1.0f - texture(colourTarget, texCoord)), 1.0f);
}

vec4 Pixelate(vec2 texCoord)
{
	float fPixels = 2048.0f;
	float dx = 15.0f * (1.0f / fPixels);
	float dy = 10.0f * (1.0f / fPixels);
	vec2 v2Coord = vec2(dx * floor(texCoord.x / dx), dy * floor(texCoord.y / dy));
	return texture(colourTarget, v2Coord);
}

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

vec4 Sharpen(vec2 texCoord)
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
		-1,  9, -1,
		-1, -1, -1
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(colourTarget, texCoord.st + offsets[i]));
	}
	vec3 col = vec3(0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0f);
}

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
	vec3 col = texture(colourTarget, tc / texSize).rgb;
	return vec4(col, 1.0f);
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