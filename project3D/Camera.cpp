#include "Camera.h"
#include "Input.h"
Camera::Camera()
{
	m_v3Position = { 10, 10, 10 };
	m_viewMatrix = glm::lookAt(m_v3Position, glm::vec3(0), glm::vec3(0, 1, 0));
	m_v3Rotation = glm::vec3(0);
}

Camera::Camera(glm::vec3 v3Position)
{
	m_v3Position = v3Position;
	m_viewMatrix = glm::lookAt(m_v3Position, glm::vec3(0), glm::vec3(0, 1, 0));
	m_v3Rotation = glm::vec3(0);
}

Camera::~Camera()
{
}

void Camera::Update(float fDeltaTime)
{
	// get input
	aie::Input* input = aie::Input::getInstance();
	glm::mat4 inverseView = glm::inverse(m_viewMatrix);


	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_v3Position -= (glm::vec3)inverseView[2] * m_fMoveSpeed * fDeltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_v3Position -= (glm::vec3)inverseView[0] * m_fMoveSpeed * fDeltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_v3Position += (glm::vec3)inverseView[2] * m_fMoveSpeed * fDeltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_v3Position += (glm::vec3)inverseView[0] * m_fMoveSpeed * fDeltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_Q))
	{
		m_v3Position -= (glm::vec3)inverseView[1] * m_fMoveSpeed * fDeltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_E))
	{
		m_v3Position += (glm::vec3)inverseView[1] * m_fMoveSpeed * fDeltaTime;
	}
	
	// move camera based on mouse input
	int nMouseX, nMouseY;
	input->getMouseDelta(&nMouseX, &nMouseY);
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_v3Position -= (glm::vec3)inverseView[0] * glm::distance(m_v3Position, glm::vec3(0, 0, 0)) * (float)nMouseX * 0.15f * fDeltaTime;
		m_v3Position -= (glm::vec3)inverseView[1] * glm::distance(m_v3Position, glm::vec3(0, 0, 0)) * (float)nMouseY * 0.15f * fDeltaTime;
	}

	// move camera in/out based on scroll
	float fScrollDelta = (float)input->getMouseScroll() - m_fLastMouseScroll;
	if (fScrollDelta != 0)
	{

		m_v3Position -= (glm::vec3)inverseView[2] * fScrollDelta * 50.f * fDeltaTime;
		m_fLastMouseScroll = (float)input->getMouseScroll();
	}

	m_viewMatrix = glm::lookAt(m_v3Position, { 0, 0, 0 }, { 0, 1, 0 });
}