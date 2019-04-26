#pragma once
#include "Shader.h"
#include "Texture.h"
#include "RenderTarget.h"

// stores all post-processing effects
enum class PostEffects
{
	none = 0,
	blur,
	chromaticAberration,
	crossHatch,
	distort,
	dof,
	edgeDetection,
	emboss,
	greyscale,
	invert,
	pixelate,
	sepia,
	sharpen,
	swirl,
	
	LENGTH
};

class ShaderChanger
{
public:
	ShaderChanger();
	~ShaderChanger();

	/*
		@brief Initialises the object; loads shaders, creates textures, etc
		@return Boolean on whether the initialisation was successful or not
	*/
	bool Init();

	/*
		@brief Swaps current shader to the specified one
		@param Shader to be changed to
		@return Boolean on whether the changing of shaders was successful or not
	*/
	bool ChangeShader(PostEffects ePostEffect);

	/*
		@brief Called each frame, processes keyboard input to change shaders
		@param Time since last frame
	*/
	void Update(float fDeltaTime);

	/*
		@brief Returns a pointer to the shader program
		@return Pointer to the shader program
	*/
	aie::ShaderProgram* GetShader() { return &m_currentShader; }

	/*
		@brief Binds uniforms to the shader program based on which post-processing effect is currently being used
		@param Width of the window
		@param Height of the window
		@param Time since the last frame
		@param Pointer to the render target
	*/
	void BindUniforms(int nWindowWidth, int nWindowHeight, float fDeltaTime, aie::RenderTarget* renderTarget);

private:
	// stores path to each post-processing fragment shader
	const char* m_shaderPaths[14];
	// stores the current post-processing effect enum, blur by default
	PostEffects m_eCurrentEffect = PostEffects::none;
	// shader for drawing the post-processing effects
	aie::ShaderProgram m_currentShader;
	// perlin noise texture for chromatic aberration post-processing effect
	aie::Texture* m_perlinTexture;
	// focal depth for lerping depth of field
	float m_fFocalDepth;
	// bools for storing whether the up/down arrow keys are currently held down
	bool m_bUpArrowDown = false;
	bool m_bDownArrowDown = false;
};