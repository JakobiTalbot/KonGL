#include "Application.h"
#include "gl_core_4_4.h"
#include "glfw/glfw3.h"
#include <iostream>
#include "Camera.h"
#include "ShaderChanger.h"


Application::Application()
{
}

Application::~Application()
{
	// delete values
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_pMesh;
	m_pMesh = nullptr;
	delete m_pShaderChanger;
	m_pShaderChanger = nullptr;
	glfwDestroyWindow(m_win);
	glfwTerminate();
}

int Application::Init()
{
	// initiate GLFW
	if (!glfwInit())
		return -1;

	// create window
	m_nWindowWidth = 1280;
	m_nWindowHeight = 720;
	m_win = glfwCreateWindow(m_nWindowWidth, m_nWindowHeight, "KonGL", nullptr, nullptr);

	// check if window was created successfully or not
	if (!m_win)
	{
		glfwTerminate();
		return -2;
	}

	// make window current context
	glfwMakeContextCurrent(m_win);

	// load GL functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_win);
		glfwTerminate();
		return -3;
	}
	
	glEnable(GL_DEPTH_TEST);

	// create variables
	if (!CreateStuff())
	{
		glfwDestroyWindow(m_win);
		glfwTerminate();
		return -4;
	}

	// create delta time
	static float fDeltaTime = 0;
	// game loop
	while (!glfwWindowShouldClose(m_win))
	{
		fDeltaTime = (float)glfwGetTime() - m_fLastTime;
		m_fDeltaTime = fDeltaTime;
		m_fLastTime = (float)glfwGetTime();
		Update(fDeltaTime);
		Draw();
		glfwSwapBuffers(m_win);
		glfwPollEvents();
	}

	// exit without error
	return 0;
}

bool Application::CreateStuff()
{
	// set background colour
	glClearColor(0.3f, 0.6f, 0.8f, 1.f);

	// set last time to 0 (used to calculate delta time)
	m_fLastTime = 0.f;

	m_bUpdateLight = true;

	// create simple camera transforms
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		GetWindowWidth() / (float)GetWindowHeight(),
		0.1f, 1000.f);

	// create objects
	m_pCamera = new Camera();
	m_pMesh = new aie::OBJMesh();
	m_fullscreenQuad.InitialiseFullscreenQuad();
	m_pShaderChanger = new ShaderChanger();
	
	// create light values
	m_light.v3Diffuse = { 1, 1, 1 };
	m_light.v3Specular = { 1, 1, 1 };
	m_v3AmbientLight = { 0.25f, 0.25f, 0.25f };

	// load vertex and fragment shaders
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	// check for error in shader
	if (!m_textureShader.link())
	{
		printf("Shader Error: %s\n", m_textureShader.getLastError());
		system("pause");
		return false;
	}

	// load mesh
	if (!m_pMesh->load("./models/model-triangulated.obj", true, true))
	{
		printf("Mesh Load Error!\n");
		system("pause");
		return false;
	}

	// create render target
	if (!m_renderTarget.initialise(1, GetWindowWidth(), GetWindowHeight()))
	{
		printf("Render Target Initialisation Error!\n");
		system("pause");
		return false;
	}

	m_renderTarget.getTarget(0).bind(0);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// set scale for mesh transform
	const int nScale = 1;
	// create mesh transform
	m_m4MeshTransform =
	{
		nScale, 0, 0, 0,
		0, nScale, 0, 0,
		0, 0, nScale, 0,
		0, 0, 0, 1
	};

	// lock cursor to screen
	m_pCamera->SetLockCursor(true);

	// set starting light direction
	m_light.v3Direction = -glm::vec3(m_pCamera->GetTransform()[2]);


	//glfwSetScrollCallback(m_win, pixelScroll);
	

	return true;
}

void Application::Update(float fDeltaTime)
{
	// update camera
	m_pCamera->Update(fDeltaTime);

	// set light to update if tab is pressed
	if (glfwGetKey(m_win, GLFW_KEY_TAB) == GLFW_PRESS && !m_bTabPressed)
	{
		m_bUpdateLight = !m_bUpdateLight;
		m_bTabPressed = true;
	}
	else
		m_bTabPressed = false;

	// update light direction to match camera direction
	if (m_bUpdateLight)
		m_light.v3Direction = -glm::vec3(m_pCamera->GetTransform()[2]);

	// if escape is pressed and cursor is locked then unlock cursor
	// if cursor is already unlocked then quit application
	if (glfwGetKey(m_win, GLFW_KEY_ESCAPE) == GLFW_PRESS && !m_bEscapePressed)
	{
		m_bEscapePressed = true;
		if (!m_pCamera->GetLockCursor())
		{
			glfwSetWindowShouldClose(m_win, true);
		}
		else
		{
			glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_pCamera->SetLockCursor(false);
		}
	}
	if (glfwGetKey(m_win, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		m_bEscapePressed = false;
	}

	// if cursor is unlocked and the screen is clicked then lock cursor
	if (glfwGetMouseButton(m_win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !m_pCamera->GetLockCursor()) {
		m_pCamera->SetLockCursor(true);
	}
	
	// reset window size if changed
	int x, y;
	glfwGetWindowSize(m_win, &x, &y);
	if (m_nWindowWidth != x
		|| m_nWindowHeight != y)
	{
		glfwSetWindowSize(m_win, m_nWindowWidth, m_nWindowHeight);
	}

	m_pShaderChanger->Update(fDeltaTime);
}

void Application::Draw()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_renderTarget.getRBO());
	// bind render target
	m_renderTarget.bind();

	// wipe the screen to the background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		GetWindowWidth() / (float)GetWindowHeight(),
		0.1f, 1000.f);

	// bind shader
	m_textureShader.bind();
	
	// bind light variables
	m_textureShader.bindUniform("Ia", m_v3AmbientLight);
	m_textureShader.bindUniform("Id", m_light.v3Diffuse);
	m_textureShader.bindUniform("Is", m_light.v3Specular);
	m_textureShader.bindUniform("LightDirection", m_light.v3Direction);

	// bind transform
	auto meshPVM = m_projectionMatrix * m_pCamera->GetView() * m_m4MeshTransform;
	m_textureShader.bindUniform("m4ProjectionViewModel", meshPVM);

	// bind normal
	m_textureShader.bindUniform("m3NormalMatrix", glm::inverseTranspose(glm::mat3(m_m4MeshTransform)));

	// bind camera position
	m_textureShader.bindUniform("cameraPosition", glm::vec3(glm::inverse(m_pCamera->GetView())[3]));

	// bind model matrix
	m_textureShader.bindUniform("m4ModelMatrix", m_m4MeshTransform);

	// draw mesh
	m_pMesh->draw();	

	m_renderTarget.unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_pShaderChanger->GetShader()->bind();
	m_pShaderChanger->BindUniforms(m_nWindowWidth, m_nWindowHeight, m_fDeltaTime, &m_renderTarget);

	m_renderTarget.getTarget(0).bind(0);

	m_fullscreenQuad.Draw();
}