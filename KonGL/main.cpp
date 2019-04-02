#include <iostream>
#include "gl_core_4_4.h"
#include "glfw/glfw3.h"
#include "Application.h"

int main()
{
	Application* app = new Application();
	int i = app->Init();

	delete app;
	app = nullptr;
	return i;
}