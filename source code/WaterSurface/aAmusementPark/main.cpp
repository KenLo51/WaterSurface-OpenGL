#include "mainWindow.h"

int main() {
	MainWindow mainWindow = MainWindow();
	mainWindow.run();
}

//#include<iostream>
//
//#include"glad/glad.h"
//#include"GLFW/glfw3.h"
//
//
//
//const unsigned int SCREEN_WIDTH = 1024;
//const unsigned int SCREEN_HEIGHT = 1024;
//
//const unsigned short OPENGL_MAJOR_VERSION = 4;
//const unsigned short OPENGL_MINOR_VERSION = 3;
//
//bool vSync = true;
//
//
//
//GLfloat vertices[] =
//{
//	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
//	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
//	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
//	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
//};
//
//GLuint indices[] =
//{
//	0, 2, 1,
//	0, 3, 2
//};
//
//
//const char* screenVertexShaderSource = R"(#version 430 core
//layout (location = 0) in vec3 pos;
//layout (location = 1) in vec2 uvs;
//out vec2 UVs;
//void main()
//{
//	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
//	UVs = uvs;
//})";
//const char* screenFragmentShaderSource = R"(#version 430 core
//out vec4 FragColor;
//uniform sampler2D screen;
//in vec2 UVs;
//void main()
//{
//	FragColor = texture(screen, UVs);
//	//FragColor = vec4(1,1,1,1);
//})";
//const char* screenComputeShaderSource = R"(#version 430 core
//layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
//layout(rgba32f) uniform image2D screen;
//void main()
//{
//	vec4 pixel = vec4(0.075, 0.133, 0.173, 1.0);
//	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
//	
//	ivec2 dims = imageSize(screen);
//	float x = -(float(pixel_coords.x * 2 - dims.x) / dims.x); // transforms to [-1.0, 1.0]
//	float y = -(float(pixel_coords.y * 2 - dims.y) / dims.y); // transforms to [-1.0, 1.0]
//	float fov = 90.0;
//	vec3 cam_o = vec3(0.0, 0.0, -tan(fov / 2.0));
//	vec3 ray_o = vec3(x, y, 0.0);
//	vec3 ray_d = normalize(ray_o - cam_o);
//	vec3 sphere_c = vec3(0.0, 0.0, -5.0);
//	float sphere_r = 1.0;
//	vec3 o_c = ray_o - sphere_c;
//	float b = dot(ray_d, o_c);
//	float c = dot(o_c, o_c) - sphere_r * sphere_r;
//	float intersectionState = b * b - c;
//	vec3 intersection = ray_o + ray_d * (-b + sqrt(b * b - c));
//	if (intersectionState >= 0.0)
//	{
//		pixel = vec4((normalize(intersection - sphere_c) + 1.0) / 2.0, 1.0);
//	}
//	imageStore(screen, pixel_coords, pixel);
//})";
//
//
//int main()
//{
//	glfwInit();
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Compute Shaders", NULL, NULL);
//	if (!window)
//	{
//		std::cout << "Failed to create the GLFW window\n";
//		glfwTerminate();
//	}
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(vSync);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize OpenGL context" << std::endl;
//	}
//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//
//
//	GLuint VAO, VBO, EBO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	GLuint screenTex;
//	glGenTextures(1, &screenTex);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, screenTex);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
//	//glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
//
//
//	GLint success;
//
//	GLuint screenVertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(screenVertexShader, 1, &screenVertexShaderSource, NULL);
//	glCompileShader(screenVertexShader);
//
//	// check if compile success
//	glGetShaderiv(screenVertexShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		// print error log
//		char infoLog[512];
//		glGetShaderInfoLog(screenVertexShader, sizeof(infoLog), NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//		exit(1);
//	}
//
//	GLuint screenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(screenFragmentShader, 1, &screenFragmentShaderSource, NULL);
//	glCompileShader(screenFragmentShader);
//
//	// check if compile success
//	glGetShaderiv(screenFragmentShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		// print error log
//		char infoLog[512];
//		glGetShaderInfoLog(screenFragmentShader, sizeof(infoLog), NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//		exit(1);
//	}
//
//	GLuint screenShaderProgram = glCreateProgram();
//	glAttachShader(screenShaderProgram, screenVertexShader);
//	glAttachShader(screenShaderProgram, screenFragmentShader);
//	glLinkProgram(screenShaderProgram);
//
//	// check if link success
//	glGetShaderiv(screenShaderProgram, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		// print error log
//		char infoLog[512];
//		glGetShaderInfoLog(screenShaderProgram, sizeof(infoLog), NULL, infoLog);
//		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
//		exit(1);
//	}
//
//
//	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
//	glShaderSource(computeShader, 1, &screenComputeShaderSource, NULL);
//	glCompileShader(computeShader);
//
//	// check if compile success
//	glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		// print error log
//		char infoLog[512];
//		glGetShaderInfoLog(computeShader, sizeof(infoLog), NULL, infoLog);
//		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
//		exit(1);
//	}
//
//	GLuint computeProgram = glCreateProgram();
//	glAttachShader(computeProgram, computeShader);
//	glLinkProgram(computeProgram);
//
//
//	int work_grp_cnt[3];
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
//	std::cout << "Max work groups per compute shader" <<
//		" x:" << work_grp_cnt[0] <<
//		" y:" << work_grp_cnt[1] <<
//		" z:" << work_grp_cnt[2] << "\n";
//
//	int work_grp_size[3];
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
//	std::cout << "Max work group sizes" <<
//		" x:" << work_grp_size[0] <<
//		" y:" << work_grp_size[1] <<
//		" z:" << work_grp_size[2] << "\n";
//
//	int work_grp_inv;
//	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
//	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";
//
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
//		glDisable(GL_DEPTH_TEST);
//
//		glUseProgram(computeProgram);
//		glBindImageTexture(5, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
//		glUniform1i(glGetUniformLocation(computeProgram, "screen"), 5);
//		glDispatchCompute(ceil(SCREEN_WIDTH / 8), ceil(SCREEN_HEIGHT / 4), 1);
//		glMemoryBarrier(GL_ALL_BARRIER_BITS);
//
//		glUseProgram(screenShaderProgram);
//		glActiveTexture(0);
//		glBindTexture(GL_TEXTURE_2D, screenTex);
//		glUniform1i(glGetUniformLocation(screenShaderProgram, "screen"), 0);
//		
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//}