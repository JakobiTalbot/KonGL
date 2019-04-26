#pragma once

#include <glm/ext.hpp>

struct GLFWwindow;
class Camera {
public:
	Camera();

	/*
		@brief Called every frame, processes mouse and keyboard input and updates the view matrix
		@param Time since last frame
	*/
	void Update(float fDeltaTime);

	/*
		@brief Returns the camera's view matrix
		@return The camera's view matrix
	*/
	glm::mat4 GetView() const { return m_m4View; }
	
	/*
		@brief Returns the camera's transform
		@return The camera's transform
	*/
	glm::mat4 GetTransform() const { return glm::inverse(m_m4View); }

	/*
		@brief Rotates the camera
		@param The vector2 to add to the camera's rotation
	*/
	void Rotate(glm::vec2 r) { m_v3Rotation += r; }
	
	/*
		@brief Moves the camera
		@param The vector3 to add to the camera's position
	*/
	void Move(glm::vec3 d) { m_v3Position += d; }

	/*
		@brief Locks/unlocks the cursor
		@param Whether to set the cursor to locked or unlocked
	*/
	void SetLockCursor(bool l);
	
	/*
		@brief Returns whether the cursor is locked or not
		@return True if the camera is locked, false if the camera is unlocked
	*/
	bool GetLockCursor() const { return m_bLockCursor; }
private:
	GLFWwindow* m_win;
	// how fast the camera moves
	float m_moveSpeed;

	// view matrix
	glm::mat4 m_m4View;
	// individual transformations
	glm::vec3 m_v3Position;
	glm::vec2 m_v3Rotation;

	// whether or not the cursor is locked to the window
	bool m_bLockCursor;
};