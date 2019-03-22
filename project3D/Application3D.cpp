#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include <GLFW/glfw3.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);
	
	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);
	m_pCamera = new Camera();
	m_pMesh = new aie::OBJMesh();
	m_quad.InitQuad();

	m_window = glfwGetCurrentContext();

	m_light.v3Diffuse = { 1, 1, 1 };
	m_light.v3Specular = { 1, 1, 1 };
	m_v3AmbientLight = { 0.25f, 0.25f, 0.25f };

	// load vertex and fragment shaders
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	// check for error in shader
	if (!m_shader.link())
	{
		printf("Shader Error: %s\n", m_shader.getLastError());
		system("pause");
		return false;
	}

	// load mesh
	if (!m_pMesh->load("./models/Dragon.obj", true, true))
	{
		printf("Mesh Load Error!\n");
		system("pause");
		return false;
	}

	// create mesh transform
	m_m4MeshTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	m_m4QuadTransform =
	{
		1.f, 0, 0, 0,
		0, 1.f, 0, 0,
		0, 0, 1.f, 0,
		0, 0, 0, 1
	};

	m_pCamera->SetLockCursor(true);

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pMesh;
	m_pMesh = nullptr;

	m_window = nullptr;
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// update camera
	m_pCamera->Update(deltaTime);

	// update light
	//m_light.v3Direction = glm::normalize(vec3(glm::cos(getTime() * 2), glm::sin(getTime() * 2), 0));
	m_light.v3Direction = -glm::vec3(m_pCamera->GetTransform()[2]);

	aie::Input* input = aie::Input::getInstance();

	// if escape is pressed and cursor is locked then unlock cursor
	// if cursor is already unlocked then quit application
	if (input->wasKeyPressed(aie::INPUT_KEY_ESCAPE))
	{
		if (!m_pCamera->GetLockCursor())
		{
			quit();
		}
		else
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_pCamera->SetLockCursor(false);
		}
	}

	// if cursor is unlocked and the screen is clicked then lock cursor
	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT) && !m_pCamera->GetLockCursor()) {
		m_pCamera->SetLockCursor(true);
	}
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// bind shader
	m_shader.bind();

	// bind transform
	auto meshPVM = m_projectionMatrix * m_pCamera->GetView() * m_m4MeshTransform;
	m_shader.bindUniform("m4ProjectionViewModel", meshPVM);

	// bind normal
	m_shader.bindUniform("m3NormalMatrix", glm::inverseTranspose(glm::mat3(m_m4MeshTransform)));

	// bind camera position
	m_shader.bindUniform("cameraPosition", glm::vec3(glm::inverse(m_pCamera->GetView())[3]));

	// bind model matrix
	m_shader.bindUniform("m4ModelMatrix", m_m4MeshTransform);

	// bind light variables
	m_shader.bindUniform("Ia", m_v3AmbientLight);
	m_shader.bindUniform("Id", m_light.v3Diffuse);
	m_shader.bindUniform("Is", m_light.v3Specular);
	m_shader.bindUniform("LightDirection", m_light.v3Direction);

	//m_quadMesh.Draw();
	m_pMesh->draw();

	//m_quad.Draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_pCamera->GetView());
	
	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}