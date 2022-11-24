#include "InputHandler.h"


InputHandler::InputHandler(GLFWwindow* window, Camera* camera) {
	InputHandler::window = window;
	InputHandler::camera = camera;
	firstClick = false;
	moveSpeed = 1.0f;

	prevState.F1 = GLFW_RELEASE;

	showGui = true;
}


void InputHandler::SetWindow(GLFWwindow* window) {
	InputHandler::window = window;
}
void InputHandler::SetCamera(Camera* camera) {
	InputHandler::camera = camera;
}

void InputHandler::Process(float deltaTime) {
	int windowWidth = camera->width;
	int windowHeight = camera->height;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && prevState.F1 == GLFW_RELEASE) {
		showGui = !showGui;
	}
	prevState.F1 = glfwGetKey(window, GLFW_KEY_F1);

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		moveSpeed = moveSpeed - (moveSpeed * 9.0f * deltaTime);
		printf("speed : %f\n", moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		moveSpeed = moveSpeed + (moveSpeed * 9.0f * deltaTime);
		printf("steed : %f\n", moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->move(glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->move(glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->move(glm::vec3(0.0f, 0.0f, -1.0f) * deltaTime * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->move(glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera->move(glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * moveSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera->move(glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime * moveSpeed);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			firstClick = false;
			glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float rotY = (mouseX - windowWidth / 2) / windowWidth * 30.0f;
		float rotX = (mouseY - windowHeight / 2) / windowHeight * 30.0f;
		camera->rotate(glm::vec2((float)rotX, (float)rotY));
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

	camera->updateMatrix();
}