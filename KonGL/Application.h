#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "RenderTarget.h"

// forward declarations
struct GLFWwindow;
class Camera;
class ShaderChanger;

class Application
{
public:
	Application();
	~Application();

	/*
		@brief Initiates the GL Application
		@return Index code of the error if there is an error, otherwise returns 0 if all is fine
	*/
	int Init();

	/*
		@brief Initialises member variables of the Application class
		@return Boolean that states if everything was created successfully or not
	*/
	bool CreateStuff();


	/*
		@brief Updates the application once every frame
		@param Time since last frame
	*/
	void Update(float fDeltaTime);

	/*
		@brief Draws the scene including shaders
	*/
	void Draw();

	/*
		@brief Returns window height
		@return Window height
	*/
	int GetWindowHeight() { return m_nWindowHeight; }

	/*
		@brief Returns window width
		@return Window width
	*/
	int GetWindowWidth() { return m_nWindowWidth; }

protected:
	// pointer to the GLFW window
	GLFWwindow* m_win;
	// the width and height of the window
	int m_nWindowHeight, m_nWindowWidth;
	// time of last frame for purpose of calculating delta time
	float m_fLastTime;
	// camera pointer
	Camera* m_pCamera;
	// projection matrix for displaying objects on screen
	glm::mat4	m_projectionMatrix;
	// shader to draw the object
	aie::ShaderProgram m_textureShader;
	// mesh of object
	aie::OBJMesh* m_pMesh;
	// transform of the mesh
	glm::mat4 m_m4MeshTransform;
	// fullscreen quad to be the target of post-processing
	Mesh m_fullscreenQuad;
	// struct for storing light variables
	struct Light
	{
		glm::vec3 v3Direction;
		glm::vec3 v3Diffuse;
		glm::vec3 v3Specular;
	};
	// light on the scene
	Light m_light;
	// ambient light colour
	glm::vec3 m_v3AmbientLight;
	// render target
	aie::RenderTarget m_renderTarget;
	// bools to make the program be able to detect when a key is pressed only once
	bool m_bEscapePressed;
	bool m_bTabPressed;
	// whether to update the light's rotation to match the camera's or not
	bool m_bUpdateLight;
	ShaderChanger* m_pShaderChanger;
	float m_fDeltaTime;
};