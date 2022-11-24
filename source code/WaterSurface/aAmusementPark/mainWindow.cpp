#include "MainWindow.h"

// call_back function while window size has been changed
void windowResize_CB(GLFWwindow* window, int width, int height) {
	currWindow->width = width;
	currWindow->height = height;
	if (currWindow->width <= 0)currWindow->width = 1;
	if (currWindow->height <= 0)currWindow->height = 1;

	currWindow->mainView->resize(currWindow->width, currWindow->height);
	//currWindow->mainView->mainCamera->setSize(currWindow->width, currWindow->height);
	//glViewport(0, 0, currWindow->width, currWindow->height);
}


// constructor. initialize window, opengl, imgui
MainWindow::MainWindow() {
	initVariables();
	initWindow();
	initOpneGL();
	initImgui();

	currWindow = this;
	glfwSetFramebufferSizeCallback(window, windowResize_CB);
	std::cout << "initialize passed" << std::endl;

	mainView = new MainView(this);
	inputHandler = new InputHandler(MainWindow::window, mainView->mainCamera);
}

void MainWindow::initVariables() {
	MainWindow::width = 1920;
	MainWindow::height = 1080;
}

// initialize glfw window
void MainWindow::initWindow() {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "GLFW initialize failed" << std::endl;
		return;
	}

	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// use OpenGL core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW creating window /////////////////////////////////////////////
	MainWindow::window = glfwCreateWindow(MainWindow::width, MainWindow::height, "Water Surface", NULL, NULL);
	if (MainWindow::window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(MainWindow::window);
}

// initialize opengl
void MainWindow::initOpneGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// tell OpenGL the size of rendering
	glViewport(0, 0, MainWindow::width, MainWindow::height);
}

// initialize imgui
void MainWindow::initImgui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(MainWindow::window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

// run window
void MainWindow::run() {
	float prevTime = glfwGetTime();

	float counter_1s = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		float currTime = glfwGetTime();
		float deltaTime = currTime - prevTime;
		prevTime = currTime;

		counter_1s = counter_1s + deltaTime;
		if (counter_1s >= 1.0f) {
			counter_1s = fmod(counter_1s, 1.0f);
			char titleStr[80];
			sprintf(titleStr, "Water Surface - FPS:%7.2f", 1.0f/ deltaTime);
			glfwSetWindowTitle(window, titleStr);
		}

		inputHandler->Process(deltaTime);

		mainView->draw(deltaTime);

		if(inputHandler->showGui)
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		mainView->updateParameter(deltaTime);

		glfwPollEvents();
	}

	std::cout << "window closed" << std::endl;

	glfwTerminate();

}

// get the width of the window
int MainWindow::w() {
	return MainWindow::width;
}

// get the height of the window
int MainWindow::h() {
	return MainWindow::height;
}