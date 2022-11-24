#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"

typedef struct {
	int F1;
}Input_prevState;

class InputHandler {
public:
	InputHandler(GLFWwindow* window=NULL, Camera* camera = NULL);
	void SetWindow(GLFWwindow* window);
	void SetCamera(Camera* camera);
	void Process(float deltaTime = 0.33f);

private:
	GLFWwindow* window;
	Camera* camera;
	bool firstClick;
	float moveSpeed;

	Input_prevState prevState;
public:
	bool showGui;
};