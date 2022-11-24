#pragma once
#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include <stb/stb_image.h>
#include <glm/glm.hpp>

#include "ShaderClass.h"

class Skybox {
private:
	void Init();


public:
	// default constructor
	Skybox();
	// constructor, find images in folder
	Skybox(const char* folder);
	// constructor use given paths
	Skybox(const char* rightFile, const char* leftFile,
		const char* topFile, const char* bottomFile,
		const char* backFile, const char* frontFile);

	// load 6 skybox images
	void loadImages(const char* rightFile, const char* leftFile,
		const char* topFile, const char* bottomFile,
		const char* backFile, const char* frontFile);
	// laod shader
	void loadShader(Shader* shader);
	// laod a new shader
	void loadShader(const char* vertex, const char* fragment);

	void Draw(glm::mat4 viewMat);

	void Delete();
private:
	static const float VertexPositions[108];

	// skybox textures, [right, left, top, bottom, front, back]
	GLuint vaoID, vboID;
	GLuint textureID;
	Shader* shader;
};

#endif