//
//// C/C++ Libraries
//#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//
//// OpenGL Libreries
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//
////
//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_glfw.h>
//#include <imgui/imgui_impl_opengl3.h>
//
//// User Defined Libreries
//#include "ShaderClass.h"
//#include "Camera.h"
//#include "Mesh.h"
//#include "Model.h"
//
//#include "InputHandler.h"
//
//
//int windowWidth = 800;
//int windowHeight = 600;
//
//Camera* mainCamera;
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	windowWidth = width;
//	windowHeight = height;
//	if (windowWidth <= 0)windowWidth = 1;
//	if (windowHeight <= 0)windowHeight = 1;
//	mainCamera->setSize(windowWidth, windowHeight);
//	glViewport(0, 0, windowWidth, windowHeight);
//}
//
//int main() {
//	// GLFW initialize /////////////////////////////////////////////
//	if (glfwInit() != GLFW_TRUE) {
//		std::cout << "GLFW initialize failed" << std::endl;
//		return 0;
//	}
//
//	// OpenGL version 3.3
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//
//	// use OpenGL core profile
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// GLFW creating window /////////////////////////////////////////////
//	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL_ReTest", NULL, NULL);
//	if (window == NULL) {
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return 0;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	// glad initialize /////////////////////////////////////////////
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	// tell OpenGL the size of rendering
//	glViewport(0, 0, windowWidth, windowHeight);
//
//	// Setup Dear ImGui context /////////////////////////////////////////////
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//
//	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
//
//	// Setup Platform/Renderer backends
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init("#version 150");
//
//	std::cout << "initialize passed" << std::endl;
//
//	// init VAO, VBO, EBO, Shader, Texture... /////////////////////////////////////////////
//	// init shader
//	//Shader objectShader("base.vert", "base.frag");
//	Shader objectShader("phong_shadow.vert", "phong_shadow.frag");
//	Shader depthShader("depth.vert", "depth.frag");
//
//	// init model
//	Model model2("testScene.obj");
//
//	// init camera
//	Camera camera1 = Camera(windowWidth, windowHeight, glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, -0.15f, -1.0f));
//	camera1.perspectiveInit(120.0f, 0.1f, 1000.0f);
//	camera1.updateMatrix();
//
//	// call the function when window has be resized
//	mainCamera = &camera1;
//	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	//
//	InputHandler inputHandler(window, &camera1);
//
//	// 
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CCW);
//	glCullFace(GL_BACK);
//
//	// init depth shader
//	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
//	Camera directionalLight = Camera(SHADOW_WIDTH, SHADOW_HEIGHT, glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
//	directionalLight.orthographicInit(10.0f/ SHADOW_WIDTH, 0.0f, 10.0f);
//	directionalLight.updateMatrix();
//
//	// configure depth map FBO
//	// -----------------------
//	unsigned int depthMapFBO;
//	glGenFramebuffers(1, &depthMapFBO);
//	// create depth texture
//	unsigned int depthMap;
//	glGenTextures(1, &depthMap);
//	glBindTexture(GL_TEXTURE_2D, depthMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//	// attach depth texture as FBO's depth buffer
//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	while (!glfwWindowShouldClose(window)) {
//		inputHandler.Process();
//
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//		ImGui::Begin("Hello, world!");
//		ImGui::Text("This is some useful text.");
//		ImGui::End();
//
//		ImGui::Render();
//
//
//		// clear frame
//		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
//		// clear buffers
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//
//		glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
//		glm::vec3 viewPos = camera1.position_vec3;
//		glm::mat4 modelTransform = glm::mat4(1.0f);
//		modelTransform = glm::scale(modelTransform, glm::vec3(0.05f, 0.05f, 0.05f));
//		modelTransform = glm::rotate(modelTransform, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//		// draw depth from light
//		// use shader
//		glCullFace(GL_FRONT);
//		depthShader.Activate();
//		depthShader.setUniform("model", modelTransform);
//		depthShader.setUniform("view", directionalLight.transform_mat4);
//		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//		glClear(GL_DEPTH_BUFFER_BIT);
//
//		model2.Draw();
//
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//		// draw objects
//		glCullFace(GL_BACK);
//		glViewport(0, 0, windowWidth, windowHeight);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		// use shader
//		objectShader.Activate();
//		objectShader.setUniform("model", modelTransform);
//		objectShader.setUniform("view", camera1.transform_mat4);
//		objectShader.setUniform("light", directionalLight.transform_mat4);
//
//
//		objectShader.setUniform("lightPos", lightPos);
//		objectShader.setUniform("viewPos", camera1.position_vec3);
//		depthShader.setUniform("shadowMap", (unsigned int)0);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, depthMap);
//
//		model2.Draw();
//		glBindTexture(GL_TEXTURE_2D, 0);
//
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//		glfwPollEvents();
//		glfwSwapBuffers(window);
//	}
//
//
//	std::cout << "window closed" << std::endl;
//
//	glfwTerminate();
//
//	return 0;
//}



#include "mainWindow.h"

int main() {
	MainWindow mainWindow = MainWindow();
	mainWindow.run();
}