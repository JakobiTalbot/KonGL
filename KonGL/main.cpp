#include <iostream>
#include "gl_core_4_4.h"
#include "glfw/glfw3.h"
#include "Application.h"
#include <crtdbg.h>

int main()
{	
	// memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Application* app = new Application();
	int i = app->Init();
	delete app;
	app = nullptr;
	return i;
}