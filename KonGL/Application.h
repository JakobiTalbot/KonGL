#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "RenderTarget.h"

// forward declarations
struct GLFWwindow;
class Camera;
class Application
{
public:
	Application();
	~Application();

	int Init();
	bool CreateStuff();

	void Update(float fDeltaTime);
	void Draw();

	int GetWindowHeight() { return m_nWindowHeight; }
	int GetWindowWidth() { return m_nWindowWidth; }

protected:
	GLFWwindow* m_win;
	int m_nWindowHeight, m_nWindowWidth;
	float m_fLastTime;

	Camera* m_pCamera;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_postProcessing;

	aie::OBJMesh* m_pMesh;
	glm::mat4 m_m4MeshTransform;

	Mesh m_fullscreenQuad;

	struct Light
	{
		glm::vec3 v3Direction;
		glm::vec3 v3Diffuse;
		glm::vec3 v3Specular;
	};

	Light m_light;
	glm::vec3 m_v3AmbientLight;

	GLFWwindow* m_window;

	aie::RenderTarget m_renderTarget;

	bool m_bEscapePressed;

};