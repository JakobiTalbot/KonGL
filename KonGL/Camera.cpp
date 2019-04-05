#include "camera.h"
#include <GLFW/glfw3.h>

Camera::Camera()
{
	m_v3Position = glm::vec3(0);
	m_v3Rotation = glm::vec3(0);
	m_m4View = glm::mat4(1);

	m_moveSpeed = 10.0f;
	SetLockCursor(true);

	m_win = glfwGetCurrentContext();
}

// upd8
void Camera::Update(float delta) 
{
	// movement
	glm::vec4 move(0);
	// get current transformation matrix for the up/right/forward vectors
	glm::mat4 trans = GetTransform();
	// forward
	if (glfwGetKey(m_win, GLFW_KEY_W) == GLFW_PRESS)
		move -= trans[2];
	// backward
	if (glfwGetKey(m_win, GLFW_KEY_S) == GLFW_PRESS)
		move += trans[2];
	// left
	if (glfwGetKey(m_win, GLFW_KEY_A) == GLFW_PRESS)
		move -= trans[0];
	// right
	if (glfwGetKey(m_win, GLFW_KEY_D) == GLFW_PRESS)
		move += trans[0];
	// up
	if (glfwGetKey(m_win, GLFW_KEY_E) == GLFW_PRESS)
		move += glm::vec4(0, 1, 0, 0);
	// down
	if (glfwGetKey(m_win, GLFW_KEY_Q) == GLFW_PRESS)
		move -= glm::vec4(0, 1, 0, 0);

	// move faster if shift is held
	if (glfwGetKey(m_win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		move *= 2;
	// move slower if ctrl is held
	else if (glfwGetKey(m_win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		move *= 0.2f;

	this->Move(glm::vec3(move) * m_moveSpeed * delta);


	if (m_bLockCursor) {
		// rotation

		// get window size for locking and stuff homie
		int wx, wy;
		glfwGetWindowSize(glfwGetCurrentContext(), &wx, &wy);

		// get current mouse position
		double _mx, _my;
		glfwGetCursorPos(glfwGetCurrentContext(), &_mx, &_my);

		// make a vector from the difference between 
		// the last and current positions
		glm::vec2 rotate(0);
		rotate.y = -(float)(_mx - (wx / 2));
		rotate.x = -(float)(_my - (wy / 2));

		// multiply by something tiny because it is crazy
		const float sens = 0.001f;
		// add to rotation :D
		this->Rotate(rotate*sens);

		// locking cursor to the middle
		glfwSetCursorPos(glfwGetCurrentContext(), wx / 2, wy / 2);
	}

	// update view matrix

	// make position matrix
	glm::mat4 posMatrix(1);
	posMatrix = glm::translate(posMatrix, m_v3Position);

	// make individual rotation matrices
	glm::mat4 rotx(1);
	glm::mat4 roty(1);
	rotx = glm::rotate(rotx, m_v3Rotation.x, { 1,0,0 });
	roty = glm::rotate(roty, m_v3Rotation.y, { 0,1,0 });
	// combine into one rotation matrix
	glm::mat4 rotMatrix = roty * rotx;

	// update view matrix
	m_m4View = glm::inverse(posMatrix * rotMatrix);
}

// does things
void Camera::SetLockCursor(bool l)
{
	GLFWwindow* win = glfwGetCurrentContext();

	m_bLockCursor = l;
	// show/hide cursor based on bool
	// GLFW_CURSOR_DISABLED hides the cursor as well as locking it
	// to the window
	glfwSetInputMode(win, GLFW_CURSOR, l ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

	// set cursor to centre of screen if unlocking cursor
	if (!l)
	{
		int wx, wy;
		glfwGetWindowSize(glfwGetCurrentContext(), &wx, &wy);
		glfwSetCursorPos(win, wx / 2, wy / 2);
	}
}