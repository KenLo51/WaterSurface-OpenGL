#include "mainView.h"

const int MainView::WATERTYPE_SINEWAVE = 0;
const int MainView::WATERTYPE_HEIGHTMAP = 1;
const int MainView::WATERTYPE_SIMULATION = 2;
const int MainView::WATER_SIMULATION_RESOLUTION = 200;

MainView::MainView(MainWindow* mw) {
	MainView::mainWindow = mw;
	initViewObjects();
	directionalLight_theta = 29.0f;
	directionalLight_phi = 67.0f;
	waterSinTheta = glm::vec2(0.0f, 0.0f);
	waterSimulationClear = false;
}

void MainView::drawImgui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// User Manual Halper
	ImGui::Begin("User Manual");
	ImGui::Text("Close Program : ESC");
	ImGui::Text("Show/Hide GUI : F1");
	ImGui::Text("Movement      : W,A,S,D");
	ImGui::Text("Movement Speed: =,-");
	ImGui::Text("rotation      : hold right mouse button down and move");
	ImGui::End();

	// References
	//ImGui::Begin("References");
	//ImGui::Text("   OpenGL Water Tutorial. https://youtu.be/HusvGeEDU_U");
	//ImGui::Text("   FLTK Project Water Surface. https://medium.com/maochinn/fltk-project-water-surface-a811c9cfc3b");
	//ImGui::Text("Textures:");
	//ImGui::Text("   Floor Tiles 09. https://polyhaven.com/a/floor_tiles_09");
	//ImGui::End();

	// Light source parameters
	ImGui::Begin("Sun light");
	ImGui::SliderFloat("theta", &directionalLight_theta, 0.0f, 360.0f);
	ImGui::SliderFloat("phi  ", &directionalLight_phi, 0.0f, 180.0f);
	ImGui::End();


	// Water parameters
	ImGui::Begin("Water");
	if (ImGui::Button("Reset")) {
		waterAmpltidue = 1.0;
		waterScale.x = waterScale.y = 1.0f;
		waterOffset.x = waterOffset.y = 0.0f;
		waterSpeed = 1.0f;
		waterSimulation->Attenuation = 0.999f;
		waterSimulation->Velocity = 0.2f;
		waterSimulation->Force = 0.045;
	}
	ImGui::RadioButton("Sine Wave", &waterType, WATERTYPE_SINEWAVE); ImGui::SameLine();
	ImGui::RadioButton("Height Map", &waterType, WATERTYPE_HEIGHTMAP); ImGui::SameLine();
	ImGui::RadioButton("Simulation", &waterType, WATERTYPE_SIMULATION);

	ImGui::SliderFloat("Ampltidue", &waterAmpltidue, 0.0f, 10.0f);
	if (waterType == WATERTYPE_SINEWAVE ||
		waterType == WATERTYPE_HEIGHTMAP) {
		ImGui::SliderFloat2("Scale", glm::value_ptr(waterScale), 0.001f, 10.0f);
		ImGui::SliderFloat2("Offset", glm::value_ptr(waterOffset), 0.0f, 2.0f);
		ImGui::SliderFloat("Speed", &waterSpeed, -10.0f, 10.0f);
	}
	if(waterType == WATERTYPE_SIMULATION) {
		// simulation can not scale & offset texture
		waterScale.x = waterScale.y = 1.0f;
		waterOffset.x = waterOffset.y = 0.0f;

		ImGui::SliderFloat("Attenuation", &(waterSimulation->Attenuation), 0.9f, 1.0f);
		ImGui::SliderFloat("Velocity", &(waterSimulation->Velocity), 0.01f, 0.7f);
		ImGui::SliderFloat("Force", &(waterSimulation->Force), -0.2f, 0.2f);

		if (ImGui::Button("Clear"))waterSimulationClear = true;
		else waterSimulationClear = false;
	}
	ImGui::Checkbox("Run", &waterRun); ImGui::SameLine();
	ImGui::Checkbox("Caustics", &enableCaustics);

	ImGui::End();


	ImGui::Render();
}

// initialize VAO, VBO, EBO, Shader, Texture...
void MainView::initViewObjects() {
	// init shader ///////////////////////////////////////////////////////////////////////////////////
	MainView::poolShader = new Shader ("pool.vert", "pool.frag");
	MainView::depthShader = new Shader("depth.vert", "depth.frag");
	MainView::waterShader = new Shader("water_heightmap.vert", "water_heightmap.frag");
	MainView::waterCausticsShader = new Shader("water_sine.vert", "water_caustics_sine.frag");
	MainView::clickposShader = new Shader("click.vert", "click.frag");

	// init camera ///////////////////////////////////////////////////////////////////////////////////
	MainView::mainCamera = new Camera(mainWindow->w(), mainWindow->h(), glm::vec3(0.0f, 5.0f, -5.0f), glm::vec3(0.0f, -1.0f, 1.0f));
	MainView::mainCamera->perspectiveInit(glm::radians(120.0f), 0.1f, 1000.0f);
	MainView::mainCamera->updateMatrix();

	// init opengl ///////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// init parameters for depth map ///////////////////////////////////////////////////////////////////////////////////
	SHADOW_RESOLUTION_prev = SHADOW_RESOLUTION = 1024;
	MainView::directionalLight = new Camera(SHADOW_RESOLUTION, SHADOW_RESOLUTION, glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
	MainView::directionalLight->orthographicInit(10.0f / SHADOW_RESOLUTION, 0.0f, 100.0f);
	MainView::directionalLight->updateMatrix();

	//		configure depth map FBO
	glGenFramebuffers(1, &depthMapFBO);
	//		create depth texture
	glGenTextures(1, &depthMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//		attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	texShadowMap = new Texture(depthMap);

	// init models ///////////////////////////////////////////////////////////////////////////////////
	glm::mat4 modelTransform;
	MainView::sceneModel = new Model("models/waterPool.obj");
	modelTransform = glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));
	MainView::sceneModel->UpdateTransform(modelTransform);

	MainView::waterModel = new Model("models/waterSurface_100x100.obj");
	modelTransform = glm::scale(glm::vec3(0.05f, 0.05f, 0.05f)) * glm::mat4(1.0f);
	modelTransform = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f)) * modelTransform;
	MainView::waterModel->UpdateTransform(modelTransform);

	// init water ///////////////////////////////////////////////////////////////////////////////////
	//MainView::texWaterColor = new Texture("models/materials/Water_001_SD/Water_001_COLOR.jpg");
	//MainView::texWaterHeight = new Texture("models/materials/waves5/198.png");
	//MainView::texWaterNormal = new Texture("models/materials/Water_001_SD/Water_001_NORM.jpg");
	MainView::aniTexWaterHeight = new AnimatedTexture("models/materials/waves5");
	MainView::waterScale = glm::vec2(1.0f, 1.0f);
	MainView::waterOffset = glm::vec2(0.0f, 0.0f);
	MainView::waterSpeed = 1.0f;
	MainView::waterAmpltidue = 1.0f;
	waterRun = false;
	waterType = WATERTYPE_SINEWAVE;

	//		underWater frame buffer
	glGenFramebuffers(1, &fboUnderwater);
	glGenTextures(1, &texUnderwaterID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texUnderwaterID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//		attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboUnderwater);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texUnderwaterID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	texUnderwater = new Texture(texUnderwaterID);

	//		water Caustics frame buffer
	glGenFramebuffers(1, &fboCaustics);
	glGenTextures(1, &texCausticsID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texCausticsID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboCaustics);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texCausticsID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	texCaustics = new Texture(texCausticsID);

	//		water refraction frame buffer
	glGenFramebuffers(1, &fboRefraction);
	glGenTextures(1, &texWaterRefractionID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texWaterRefractionID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainView::mainWindow->w(), MainView::mainWindow->h(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboRefraction);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texWaterRefractionID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	texWaterRefraction = new Texture(texWaterRefractionID);

	//		water reflaction frame buffer
	glGenFramebuffers(1, &fboReflection);
	glGenTextures(1, &texWaterReflectionID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texWaterReflectionID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainView::mainWindow->w(), MainView::mainWindow->h(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboReflection);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texWaterReflectionID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	texWaterReflection = new Texture(texWaterReflectionID);

	//		water click position frame buffer
	glGenFramebuffers(1, &fboClickpos);
	glGenTextures(1, &texClickposID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texClickposID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainView::mainWindow->w(), MainView::mainWindow->h(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboClickpos);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texClickposID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	texClickpos = new Texture(texClickposID);

	// skybox /////////////////////////////////////////////////////////////////////////////////////////
	skybox = new Skybox("models/materials/skybox");
	skybox->loadShader("skybox.vert", "skybox.frag");

	// init simulation /////////////////////////////////////////////////////////////////////////////////////////
	waterSimulation = new WaveSimulation(WATER_SIMULATION_RESOLUTION, WATER_SIMULATION_RESOLUTION);
	glGenTextures(1, &TexSimulationWaterHeightID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexSimulationWaterHeightID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WATER_SIMULATION_RESOLUTION, WATER_SIMULATION_RESOLUTION, 0, GL_RED, GL_FLOAT, waterSimulation->datas);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	TexSimulationWaterHeight = new Texture(TexSimulationWaterHeightID);
}

void MainView::resize(int width, int height) {
	MainView::mainCamera->setSize(width, height);

	//		water refraction frame buffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texWaterRefractionID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboRefraction);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texWaterRefractionID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//		water reflaction frame buffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texWaterReflectionID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboReflection);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texWaterReflectionID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//		water click position frame buffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texClickposID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainView::mainWindow->w(), MainView::mainWindow->h(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboClickpos);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texClickposID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0, 0, width, height);
}

void MainView::drawView(float deltaTime) {

	// update objects ////////////////////////////////////////////
	glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
	glm::vec3 viewPos = mainCamera->position_vec3;
	glm::mat4 modelTransform = glm::mat4(1.0f);

	// update light ////////////////////////////////////////////
	glm::vec3 lightOrientation = glm::vec3(0.0f, -1.0f, 0.0f);
	lightOrientation = glm::rotateX(lightOrientation, glm::radians(directionalLight_phi));
	lightOrientation = glm::rotateY(lightOrientation, glm::radians(directionalLight_theta));
	MainView::directionalLight->orientation_vec3 = lightOrientation;
	MainView::directionalLight->position_vec3 = -lightOrientation*20.0f;
	glm::vec3 lightUp = glm::vec3(0.0f, 0.0f, 1.0f);
	lightUp = glm::rotateX(lightUp, glm::radians(directionalLight_phi));
	lightUp = glm::rotateY(lightUp, glm::radians(directionalLight_theta));
	MainView::directionalLight->Up = lightUp;
	MainView::directionalLight->updateMatrix();


	// draw objects //////////////////////////////////////////////
	glDisable(GL_BLEND);
	// clear frame
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// draw depth map /////////////////////////////////////
	//		use shader
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	MainView::depthShader->Activate();
	MainView::depthShader->setUniform("view", directionalLight->transform_mat4);
	sceneModel->Draw(*MainView::depthShader);
	Texture::UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// draw Caustics map /////////////////////////////////////
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	glBindFramebuffer(GL_FRAMEBUFFER, fboCaustics);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (MainView::waterType == MainView::WATERTYPE_SINEWAVE) 
		MainView::waterCausticsShader->setShadersSource("water_sine.vert", "water_caustics_sine.frag");
	if (MainView::waterType == MainView::WATERTYPE_HEIGHTMAP ||
		MainView::waterType == MainView::WATERTYPE_SIMULATION)
		MainView::waterCausticsShader->setShadersSource("water_heightmap.vert", "water_caustics_heightmap.frag");
	MainView::waterCausticsShader->Activate();
	glm::mat4 resizeWaterSurfaceTransform = glm::scale(glm::vec3(1.01f, 1.0f, 1.01f)) * MainView::waterModel->transform;
	MainView::waterCausticsShader->setUniform("model", MainView::waterModel->transform);
	MainView::waterCausticsShader->setUniform("view", directionalLight->transform_mat4);
	if (MainView::waterType == WATERTYPE_SINEWAVE) {
		if (MainView::waterRun) waterSinTheta += deltaTime * MainView::waterSpeed * 0.1f;
		MainView::waterCausticsShader->setUniform("theta", waterSinTheta);
	}
	if (MainView::waterType == WATERTYPE_HEIGHTMAP) {
		MainView::aniTexWaterHeight->BindAndUse(*MainView::waterCausticsShader, "texHeight", MainView::waterRun ? deltaTime * MainView::waterSpeed : 0.0f);
	}
	if (MainView::waterType == MainView::WATERTYPE_SIMULATION) {
		MainView::TexSimulationWaterHeight->BindAndUse(*MainView::waterShader, "texHeight");
	}
	MainView::waterCausticsShader->setUniform("ampltidue", MainView::waterAmpltidue);
	MainView::waterCausticsShader->setUniform("texOffset", MainView::waterOffset);
	MainView::waterCausticsShader->setUniform("texScale", MainView::waterScale);
	waterModel->Draw();
	Texture::UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// draw under-water map /////////////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, fboUnderwater);
	glViewport(0, 0, 1024, 1024);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (MainView::waterType == MainView::WATERTYPE_SINEWAVE)
		MainView::waterShader->setShadersSource("water_sine.vert", "depth.frag");
	if (MainView::waterType == MainView::WATERTYPE_HEIGHTMAP || 
		MainView::waterType == MainView::WATERTYPE_SIMULATION)
		MainView::waterShader->setShadersSource("water_heightmap.vert", "depth.frag");
	MainView::waterShader->Activate();
	MainView::waterShader->setUniform("model", resizeWaterSurfaceTransform);
	Camera lookDown = Camera(1024, 1024, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lookDown.Up = glm::vec3(0.0, 0.0, 1.0);
	lookDown.orthographicInit(0.01f, 0.0f, 10.0f);
	lookDown.updateMatrix();
	MainView::waterShader->setUniform("view", lookDown.transform_mat4);
	if (MainView::waterType == WATERTYPE_SINEWAVE) {
		if (MainView::waterRun) waterSinTheta += deltaTime * MainView::waterSpeed * 0.1f;
		MainView::waterShader->setUniform("theta", waterSinTheta);
	}
	if (MainView::waterType == WATERTYPE_HEIGHTMAP) {
		MainView::aniTexWaterHeight->BindAndUse(*MainView::waterShader, "texHeight", MainView::waterRun ? deltaTime * MainView::waterSpeed : 0.0f);
	}
	if (MainView::waterType == MainView::WATERTYPE_SIMULATION) {
		MainView::TexSimulationWaterHeight->BindAndUse(*MainView::waterShader, "texHeight");
	}

	MainView::waterShader->setUniform("ampltidue", MainView::waterAmpltidue);
	MainView::waterShader->setUniform("texOffset", MainView::waterOffset);
	MainView::waterShader->setUniform("texScale", MainView::waterScale);
	waterModel->Draw();
	Texture::UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// draw water pool ////////////////////////////////////
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, MainView::mainWindow->w(), MainView::mainWindow->h());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//		use shader
	MainView::poolShader->Activate();
	MainView::poolShader->setUniform("view", MainView::mainCamera->transform_mat4);
	MainView::poolShader->setUniform("light", MainView::directionalLight->transform_mat4);

	MainView::poolShader->setUniform("lightPos", MainView::directionalLight->position_vec3);
	MainView::poolShader->setUniform("viewPos", MainView::mainCamera->position_vec3);

	MainView::poolShader->setUniform("underwaterView", lookDown.transform_mat4);
	MainView::poolShader->setUniform("enableCaustics", enableCaustics);
	texShadowMap->BindAndUse(*MainView::poolShader, "shadowMap");
	texCaustics->BindAndUse(*MainView::poolShader, "CausticsMap");
	texUnderwater->BindAndUse(*MainView::poolShader, "underWaterMap");
	sceneModel->Draw(*MainView::poolShader);
	Texture::UnBind();

	// draw water surface /////////////////////////////////
	//		draw refraction
	glBindFramebuffer(GL_FRAMEBUFFER, fboRefraction);

	glCullFace(GL_BACK);
	glViewport(0, 0, MainView::mainCamera->width, MainView::mainCamera->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 
	//glm::vec4 clipPlan = glm::vec4(0, -0, 0, 2);
	//MainView::waterRefractionShader->Activate();
	//MainView::waterRefractionShader->setUniform("clipDistance0", clipPlan);
	//MainView::waterRefractionShader->setUniform("view", MainView::mainCamera->transform_mat4);
	//MainView::waterRefractionShader->setUniform("light", MainView::directionalLight->transform_mat4);

	//MainView::waterRefractionShader->setUniform("lightPos", MainView::directionalLight->position_vec3);
	//MainView::waterRefractionShader->setUniform("viewPos", MainView::mainCamera->position_vec3);

	//texShadowMap->BindAndUse(*MainView::waterRefractionShader, "shadowMap");
	//sceneModel->Draw(*MainView::waterRefractionShader);
	//Texture::UnBind();

	Camera refractionView = Camera(MainView::mainCamera->width, MainView::mainCamera->height, MainView::mainCamera->position_vec3, MainView::mainCamera->orientation_vec3);
	float refractionViewY = glm::angle(refractionView.Up, refractionView.orientation_vec3);
	if (refractionViewY < glm::radians(120.0f) && refractionView.position_vec3.y > 2.0f)
		refractionView.orientation_vec3 = glm::rotate(glm::radians(120.0f), glm::cross(refractionView.Up, refractionView.orientation_vec3)) * glm::vec4(0, 1, 0, 1);
	refractionView.perspectiveInit(glm::radians(120.0f), 0.1f, 1000.0f);
	refractionView.updateMatrix();

	skybox->Draw(MainView::mainCamera->transform_mat4);

	MainView::poolShader->Activate();
	//MainView::poolShader->setUniform("view", MainView::mainCamera->transform_mat4);
	MainView::poolShader->setUniform("view", refractionView.transform_mat4);
	MainView::poolShader->setUniform("light", MainView::directionalLight->transform_mat4);

	MainView::poolShader->setUniform("lightPos", MainView::directionalLight->position_vec3);
	MainView::poolShader->setUniform("viewPos", MainView::mainCamera->position_vec3);
	MainView::poolShader->setUniform("enableCaustics", enableCaustics);
	texCaustics->BindAndUse(*MainView::poolShader, "CausticsMap");
	texUnderwater->BindAndUse(*MainView::poolShader, "underWaterMap");
	texShadowMap->BindAndUse(*MainView::poolShader, "shadowMap");
	sceneModel->Draw(*MainView::poolShader);
	Texture::UnBind();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//		draw reflection
	glBindFramebuffer(GL_FRAMEBUFFER, fboReflection);

	glCullFace(GL_BACK);
	glViewport(0, 0, MainView::mainCamera->width, MainView::mainCamera->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 
	Camera cameraReflection(MainView::mainCamera->width, MainView::mainCamera->height, MainView::mainCamera->position_vec3, MainView::mainCamera->orientation_vec3);
	cameraReflection.perspectiveInit(glm::radians(120.0f), 0.1f, 1000.0f);
	cameraReflection.orientation_vec3.y = -cameraReflection.orientation_vec3.y;
	cameraReflection.Up = -(MainView::mainCamera->Up);
	cameraReflection.position_vec3.y = 2.0f - (cameraReflection.position_vec3.y - 2.0f);
	cameraReflection.updateMatrix();
	skybox->Draw(cameraReflection.transform_mat4);
	MainView::poolShader->Activate();
	MainView::poolShader->setUniform("view", cameraReflection.transform_mat4);
	MainView::poolShader->setUniform("light", MainView::directionalLight->transform_mat4);

	MainView::poolShader->setUniform("lightPos", MainView::directionalLight->position_vec3);
	MainView::poolShader->setUniform("viewPos", MainView::mainCamera->position_vec3);
	
	texShadowMap->BindAndUse(*MainView::poolShader, "shadowMap");
	sceneModel->Draw(*MainView::poolShader);
	Texture::UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//		draw water surface
	glViewport(0, 0, MainView::mainWindow->w(), MainView::mainWindow->h());
	glDisable(GL_CLIP_DISTANCE0);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (MainView::waterType == MainView::WATERTYPE_SINEWAVE)
		MainView::waterShader->setShadersSource("water_sine.vert", "water_sine.frag");
	if (MainView::waterType == MainView::WATERTYPE_HEIGHTMAP ||
		MainView::waterType == MainView::WATERTYPE_SIMULATION) {
		MainView::waterShader->setShadersSource("water_heightmap.vert", "water_heightmap.frag");
	}
	MainView::waterShader->Activate();
	MainView::waterShader->setUniform("model", MainView::waterModel->transform);
	MainView::waterShader->setUniform("view", MainView::mainCamera->transform_mat4);
	MainView::waterShader->setUniform("light", MainView::directionalLight->transform_mat4);
	MainView::waterShader->setUniform("refractionView", refractionView.transform_mat4);

	MainView::waterShader->setUniform("lightPos", MainView::directionalLight->position_vec3);
	MainView::waterShader->setUniform("viewPos", MainView::mainCamera->position_vec3);
	texShadowMap->BindAndUse(*MainView::waterShader, "shadowMap");
	if (MainView::waterType == WATERTYPE_SINEWAVE) {
		if(MainView::waterRun) waterSinTheta += deltaTime * MainView::waterSpeed* 0.1f;
		MainView::waterShader->setUniform("theta", waterSinTheta);
	}
	if (MainView::waterType == WATERTYPE_HEIGHTMAP) {
		MainView::aniTexWaterHeight->BindAndUse(*MainView::waterShader, "texHeight", MainView::waterRun ? deltaTime * MainView::waterSpeed : 0.0f);
		MainView::waterShader->setUniform("texRes", (int)1024);
	}
	if (MainView::waterType == MainView::WATERTYPE_SIMULATION) {
		MainView::TexSimulationWaterHeight->BindAndUse(*MainView::waterShader, "texHeight");
		MainView::waterShader->setUniform("texRes", (int)MainView::WATER_SIMULATION_RESOLUTION);
	}
	texWaterRefraction->BindAndUse(*MainView::waterShader, "texRefraction");
	texWaterReflection->BindAndUse(*MainView::waterShader, "texReflection");
	MainView::waterShader->setUniform("ampltidue", MainView::waterAmpltidue);
	MainView::waterShader->setUniform("texOffset", MainView::waterOffset);
	MainView::waterShader->setUniform("texScale", MainView::waterScale);
	waterModel->Draw();
	Texture::UnBind();


	// draw skybox
	skybox->Draw(mainCamera->transform_mat4);
}

void MainView::draw(float deltaTime) {
	// draw imgui
	if(mainWindow->inputHandler->showGui)
		drawImgui();

	// draw view
	drawView(deltaTime);
}

void MainView::updateParameter(float deltaTime) {
	if (waterSimulationClear) {
		waterSimulation->Clear();
	}
	if ((MainView::waterType == MainView::WATERTYPE_SIMULATION) && MainView::waterRun) {
		if (glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			double mouseX, mouseY;
			glfwGetCursorPos(mainWindow->window, &mouseX, &mouseY);
			mouseX = mouseX;
			mouseY = (float)mainWindow->h() - mouseY;

			//printf("%7.2d-%7.2d\n", mainWindow->w(), mainWindow->h());
			//printf("%7.2lf-%7.2lf\n", mouseX, mouseY);

			glBindBuffer(GL_FRAMEBUFFER, fboClickpos);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			MainView::clickposShader->Activate();
			MainView::clickposShader->setUniform("view", MainView::mainCamera->transform_mat4);
			MainView::clickposShader->setUniform("model", MainView::waterModel->transform);
			MainView::waterModel->Draw();
			
			float data[4];
			glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
			glReadPixels(mouseX, mouseY, 1, 1, GL_RGBA, GL_FLOAT, data);

			glBindBuffer(GL_FRAMEBUFFER, 0);

			//printf("  %7.2lf-%7.2lf-%7.2lf\n", data[0], data[1], data[2]);
			
			if (data[2] == 1.0f) {
				waterSimulation->Add(MainView::WATER_SIMULATION_RESOLUTION* data[0], MainView::WATER_SIMULATION_RESOLUTION * data[1], 0.1);
			}
		}
		waterSimulation->Update();
		glBindTexture(GL_TEXTURE_2D, TexSimulationWaterHeightID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WATER_SIMULATION_RESOLUTION, WATER_SIMULATION_RESOLUTION, 0, GL_RED, GL_FLOAT, waterSimulation->datas);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


WaveSimulation::WaveSimulation(int width, int height) {
	WaveSimulation::width = width;
	WaveSimulation::height = height;

	WaveSimulation::datas = new float[width * height];
	WaveSimulation::diff1 = new float[width * height];
	WaveSimulation::diff2 = new float[width * height];

	for (unsigned int i = 0; i < width * height; i++) {
		WaveSimulation::datas[i] = 0.5f;
		WaveSimulation::diff1[i] = 0.0f;
		WaveSimulation::diff2[i] = 0.0f;
	}
}

void WaveSimulation::Add(int x, int y, float amp) {
	const int r = Radius;
	for (int row = - r; row <= + r; row++) {
		if (((y + row) < 0) || ((y + row) >= height)) continue;
		for (int col = - r; col <= + r; col++) {
			if (((x + col) < 0) || ((x + col) >= width)) continue;
			float l = (row * row + col * col);
			if (row == 0 && col == 0) l = 0.5;
			WaveSimulation::diff1[(y+row) * WaveSimulation::width + (x+col)] +=	amp * WaveSimulation::Force / l;
		}
	}
	//WaveSimulation::diff1[y * WaveSimulation::width + x] += amp;
}

void WaveSimulation::Update() {
	for (int row = 0; row < WaveSimulation::height; row++) {
		for (int col = 0; col < WaveSimulation::width; col++) {
			//acc[row, col] += (valuesPad[row + 2, col + 1] - valuesPad[row + 1, col + 1]) - (valuesPad[row + 1, col + 1] - valuesPad[row + 0, col + 1])
			//acc[row, col] += (valuesPad[row + 1, col + 2] - valuesPad[row + 1, col + 1]) - (valuesPad[row + 1, col + 1] - valuesPad[row + 1, col + 0])

			//WaveSimulation::diff2[row * WaveSimulation::width + col] +=
			//	((((row + 1) < WaveSimulation::height) ? datas[(row + 1) * WaveSimulation::width + col + 0] : 0.5f) - datas[(row + 0) * WaveSimulation::width + col + 0]) -
			//	(datas[(row + 0) * WaveSimulation::width + col + 0] - (((row - 1) >= 0) ? datas[(row - 1) * WaveSimulation::width + col + 0] : 0.5f));
			//WaveSimulation::diff2[row * WaveSimulation::width + col] +=
			//	((((col + 1) < WaveSimulation::width) ? datas[(row + 0) * WaveSimulation::width + col + 1] : 0.5f) - datas[(row + 0) * WaveSimulation::width + col + 0]) -
			//	(datas[(row + 0) * WaveSimulation::width + col + 0] - (((col -1) >= 0) ? datas[(row + 0) * WaveSimulation::width + col - 1] : 0.5f));

			//WaveSimulation::diff2[row * WaveSimulation::width + col] += 0.0001 * (0.5 - WaveSimulation::datas[row * WaveSimulation::width + col]);
			WaveSimulation::diff2[row * WaveSimulation::width + col] +=
				((((row + 1) < WaveSimulation::height) ? datas[(row + 1) * WaveSimulation::width + col + 0] : datas[(WaveSimulation::height - 1) * WaveSimulation::width + col + 0]) - datas[(row + 0) * WaveSimulation::width + col + 0]) -
				(datas[(row + 0) * WaveSimulation::width + col + 0] - (((row - 1) >= 0) ? datas[(row - 1) * WaveSimulation::width + col + 0] : datas[(0) * WaveSimulation::width + col + 0]));
			WaveSimulation::diff2[row * WaveSimulation::width + col] +=
				((((col + 1) < WaveSimulation::width) ? datas[(row + 0) * WaveSimulation::width + col + 1] : datas[(row + 0) * WaveSimulation::width + WaveSimulation::width - 1]) - datas[(row + 0) * WaveSimulation::width + col + 0]) -
				(datas[(row + 0) * WaveSimulation::width + col + 0] - (((col - 1) >= 0) ? datas[(row + 0) * WaveSimulation::width + col - 1] : datas[(row + 0) * WaveSimulation::width + 0]));
		}
	}

	for (unsigned int i = 0; i < WaveSimulation::width * WaveSimulation::height; i++) {
		WaveSimulation::diff2[i] = WaveSimulation::diff2[i] * WaveSimulation::Velocity * WaveSimulation::Velocity
			+ (0.01f) * (0.5f - WaveSimulation::datas[i]);
		WaveSimulation::diff1[i] = WaveSimulation::diff1[i] * WaveSimulation::Attenuation + WaveSimulation::diff2[i];
		WaveSimulation::datas[i] = WaveSimulation::datas[i] + WaveSimulation::diff1[i];
	}

	for (unsigned int i = 0; i < WaveSimulation::width * WaveSimulation::height; i++) {
		WaveSimulation::diff2[i] = 0.0f;
	}
}

void WaveSimulation::Clear() {
	for (unsigned int i = 0; i < width * height; i++) {
		WaveSimulation::datas[i] = 0.5f;
		WaveSimulation::diff1[i] = 0.0f;
		WaveSimulation::diff2[i] = 0.0f;
	}
}

void WaveSimulation::Delete() {
	delete []WaveSimulation::datas;
	delete []WaveSimulation::diff1;
	delete []WaveSimulation::diff2;
}