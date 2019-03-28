#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "RenderTarget.h"

class Camera;

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
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
};