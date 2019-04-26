#include "ShaderChanger.h"
#include "gl_core_4_4.h"
#include "glfw/glfw3.h"
#include "Application.h"
ShaderChanger::ShaderChanger()
{
	Init();
}

ShaderChanger::~ShaderChanger()
{
	delete m_perlinTexture;
	m_perlinTexture = nullptr;
}

bool ShaderChanger::Init()
{
	// load shader
	m_currentShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post/post.vert");
	// try to link shader
	if (!m_currentShader.link())
	{
		// link has failed, print error
		printf("Post-processing shader link error: %s\n", m_currentShader.getLastError());
		system("pause");
		return false;
	}
	// set shader paths
	m_shaderPaths[0] = "./shaders/post/nopost.frag";
	m_shaderPaths[1] = "./shaders/post/blur.frag";
	m_shaderPaths[2] = "./shaders/post/chromaticaberration.frag";
	m_shaderPaths[3] = "./shaders/post/crosshatch.frag";
	m_shaderPaths[4] = "./shaders/post/distort.frag";
	m_shaderPaths[5] = "./shaders/post/dof.frag";
	m_shaderPaths[6] = "./shaders/post/edgedetection.frag";
	m_shaderPaths[7] = "./shaders/post/emboss.frag";
	m_shaderPaths[8] = "./shaders/post/greyscale.frag";
	m_shaderPaths[9] = "./shaders/post/invert.frag";
	m_shaderPaths[10] = "./shaders/post/pixelate.frag";
	m_shaderPaths[11] = "./shaders/post/sepia.frag";
	m_shaderPaths[12] = "./shaders/post/sharpen.frag";
	m_shaderPaths[13] = "./shaders/post/swirl.frag";

	// set current shader
	ChangeShader(m_eCurrentEffect);

	// create texture
	m_perlinTexture = new aie::Texture();
	// load perlin noise texture for chromatic aberration
	m_perlinTexture->load("./perlin.png");

	// no errors
	return true;
}

bool ShaderChanger::ChangeShader(PostEffects ePostEffect)
{
	// load shader
	m_currentShader.loadShader(aie::eShaderStage::FRAGMENT, m_shaderPaths[(int)ePostEffect]);
	// try to link shader
	if (!m_currentShader.link())
	{
		// link has failed, print error
		printf("Post-processing shader link error: %s\n", m_currentShader.getLastError());
		system("pause");
		return false;
	}
	// set title to current post-processing effect
	glfwSetWindowTitle(glfwGetCurrentContext(), m_shaderPaths[(int)ePostEffect]);
	m_eCurrentEffect = ePostEffect; //  set member post-processing effect
	// load has succeeded
	return true;
}

void ShaderChanger::Update(float fDeltaTime)
{
	// change shader when up is pressed
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS
		&& !m_bUpArrowDown)
	{
		// wrap around to first shader if on last shader
		if ((int)m_eCurrentEffect == (int)PostEffects::LENGTH - 1)
			ChangeShader((PostEffects)0);
		else // set shader to next in array
			ChangeShader((PostEffects)((int)m_eCurrentEffect + 1));
		// set bool so program doesn't change shaders multiple times
		m_bUpArrowDown = true;
	}
	// set bool when key is released so we can change shader again
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_RELEASE)
		m_bUpArrowDown = false;

	// change shader when down is pressed
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS
			 && !m_bDownArrowDown)
	{
		// wrap around to last shader if on first shader
		if ((int)m_eCurrentEffect == 0)
			ChangeShader((PostEffects)((int)PostEffects::LENGTH - 1));
		else // set shader to previous in array
			ChangeShader((PostEffects)((int)m_eCurrentEffect - 1));
		// set bool so program doesn't change shaders multiple times
		m_bDownArrowDown = true;
	}
	// set bool when key is released so we can change shader again
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_RELEASE)
		m_bDownArrowDown = false;
}

void ShaderChanger::BindUniforms(int nWindowWidth, int nWindowHeight, float fDeltaTime, aie::RenderTarget* renderTarget)
{
	// bind render texture
	m_currentShader.bindUniform("renderTex", 0);

	// bind post-processing uniforms

	// bind uniforms for chromatic aberration effect
	if (m_eCurrentEffect == PostEffects::chromaticAberration)
	{
		m_currentShader.bindUniform("noiseTexture", 7);
		m_perlinTexture->bind(7);
		m_currentShader.bindUniform("distortionTime", (float)glfwGetTime());
	}
	// bind uniforms for depth of field effect
	else if (m_eCurrentEffect == PostEffects::dof)
	{
		// create array containing each pixel on the screen
		float* fPixels = new float[(int)(nWindowWidth * nWindowHeight)];
		glBindTexture(GL_TEXTURE_2D, renderTarget->getRBO());
		glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, fPixels);
		// set linearised focal depth
		float f = -100.0f * 0.1f / (fPixels[((nWindowHeight / 2) * nWindowWidth) + (nWindowWidth / 2)] * (100.0f - 0.1f) - 100.0f);
		// get distance between depths
		float fDistance = f - m_fFocalDepth;
		// if the focal depth isn't already very close to the target focal depth
		if (fabsf(fDistance) > 0.03f)
			m_fFocalDepth += fDistance * fDeltaTime * 7.f; // lerp
		else
			m_fFocalDepth = f;
		delete[] fPixels;
		m_currentShader.bindUniform("focalDepth", m_fFocalDepth);
		m_currentShader.bindUniform("depthTex", 1);
	}
	// bind uniforms for swirl effect
	else if (m_eCurrentEffect == PostEffects::swirl)
	{
		m_currentShader.bindUniform("width", nWindowWidth);
		m_currentShader.bindUniform("height", nWindowHeight);
		m_currentShader.bindUniform("centre", glm::vec2(nWindowWidth / 2, nWindowHeight / 2));
	}
}