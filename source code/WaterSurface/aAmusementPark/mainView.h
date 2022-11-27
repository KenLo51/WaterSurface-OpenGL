#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

// OpenGL Libreries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// User Defined Libreries
#include "ShaderClass.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "AnimatedTexture.h"
#include "Skybox.h"

#include "InputHandler.h"

#include "mainWindow.h"

class MainWindow;

class WaveSimulation;

class MainView {
private:
	void drawImgui();

	// initialize VAO, VBO, EBO, Shader, Texture...
	void initViewObjects();
	void drawView(float deltaTime = 1.0f);
public:
	MainView(MainWindow* mw = NULL);
	void draw(float deltaTime = 1.0f);
	void resize(int width, int height);
	void updateParameter(float deltaTime = 1.0f);

private:
	int SHADOW_RESOLUTION_prev;
	int SHADOW_RESOLUTION;
	unsigned int depthMapFBO;
	unsigned int depthMap;
public:
	MainWindow* mainWindow;
	bool limitFrameRate;

	//camears
	Camera* mainCamera;

	//models
	Model* sceneModel;
	Model* waterModel;
	// water parameter
	int waterType;
	const static int WATERTYPE_SINEWAVE;
	const static int WATERTYPE_HEIGHTMAP;
	const static int WATERTYPE_SIMULATION;
	const static int WATER_SIMULATION_RESOLUTION;
	bool waterRun;
	glm::vec2 waterScale;
	glm::vec2 waterOffset;
	float waterSpeed;
	glm::vec2 waterSinTheta;
	float waterAmpltidue;


	WaveSimulation* waterSimulation;
	bool waterSimulationClear;

	// frame buffers
	GLuint fboRefraction;
	Texture* texWaterRefraction;
	GLuint texWaterRefractionID;

	GLuint fboReflection;
	Texture* texWaterReflection;
	GLuint texWaterReflectionID;

	GLuint fboUnderwater;
	Texture* texUnderwater;
	GLuint texUnderwaterID;

	GLuint fboCaustics;
	Texture* texCaustics;
	GLuint texCausticsID;
	bool enableCaustics;

	GLuint fboClickpos;
	Texture* texClickpos;
	GLuint texClickposID;

	//textures
	AnimatedTexture* aniTexWaterHeight;
	Texture* texShadowMap;
	Texture* TexSimulationWaterHeight;
	GLuint TexSimulationWaterHeightID;

	//lights
	Camera* directionalLight;
	float directionalLight_theta;
	float directionalLight_phi;

	//shaders
	Shader* poolShader;
	Shader* depthShader;
	Shader* waterShader;
	//Shader* waterRefractionShader;
	Shader* waterCausticsShader;
	Shader* clickposShader;

	// skybox
	Skybox* skybox;
};

// ComputeShader WaveSimulation
class WaveSimulation {
private:
public:
	WaveSimulation(int width, int height);
	void Add(int x, int y, float amp);
	void Update(int x, int y, float amp, float deltaTime);
	void Clear();
	void Delete();
private:
	GLuint diff1Tex[2];
	GLuint computeShader, computeProgram;
public:
	unsigned int currTex;
	unsigned int width;
	unsigned int height;
	GLuint dataTex[2];

	float Attenuation = 0.999f;
	float Velocity = 0.2f;
	float Force = 0.045;
	int Radius = 5;
};