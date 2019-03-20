#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class Camera
{
public:
	Camera();
	Camera(glm::vec3 v3Position);
	~Camera();

	void Update(float fDeltaTime);

	glm::mat4 GetViewMatrix() { return glm::lookAt(m_v3Position, glm::vec3(0), glm::vec3(0, 1, 0)); }

protected:
	glm::mat4 m_viewMatrix;
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Rotation;
	float m_fMoveSpeed = 10.f;
	float m_fRotateSpeed = 10.f;
	float m_fLastMouseScroll = 0.f;
};