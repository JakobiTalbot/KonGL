#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"

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

	aie::ShaderProgram m_shader;

	aie::OBJMesh* m_pMesh;
	glm::mat4 m_m4MeshTransform;
};