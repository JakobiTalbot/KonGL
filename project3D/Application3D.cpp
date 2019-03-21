#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"

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
	if (!m_pMesh->load("./models/soulspear.obj", true, true))
	{
		printf("Mesh Load Error!\n");
		system("pause");
		return false;
	}

	// create mesh transform
	m_m4MeshTransform =
	{
		1.f, 0, 0, 0,
		0, 1.f, 0, 0,
		0, 0, 1.f, 0,
		0, 0, 0, 1
	};

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pMesh;
	m_pMesh = nullptr;
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
	m_pCamera->Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
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
	auto meshPVM = m_projectionMatrix * m_pCamera->GetViewMatrix() * m_m4MeshTransform;
	m_shader.bindUniform("m4ProjectionViewModel", meshPVM);

	//m_quadMesh.Draw();
	m_pMesh->draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_pCamera->GetViewMatrix());
	
	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}