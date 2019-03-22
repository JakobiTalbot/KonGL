#pragma once

#include <glm/ext.hpp>

class Camera {
public:
	Camera();

	// called every frame to process mouse and keyboard inputs
	// and update the view matrix
	void Update(float delta);

	// the current view matrix
	glm::mat4 GetView() const { return m_m4View; }
	// the transformation matrix
	glm::mat4 GetTransform() const { return glm::inverse(m_m4View); }

	// add to rotation
	void Rotate(glm::vec2 r) { m_v3Rotation += r; }
	// add to position
	void Move(glm::vec3 d) { m_v3Position += d; }

	// sets whether or not the cursor is captured by the game
	void SetLockCursor(bool l);
	// returns whether or not the cursor is currently locked to the window
	bool GetLockCursor() const { return m_bLockCursor; }
private:
	// how fast move.
	float m_moveSpeed;

	// view matrix
	glm::mat4 m_m4View;
	// individual transformations
	glm::vec3 m_v3Position;
	glm::vec2 m_v3Rotation;

	// whether or not the cursor is locked to the window
	bool m_bLockCursor;
};