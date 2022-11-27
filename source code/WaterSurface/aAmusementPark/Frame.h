#pragma once
#ifndef FRAME_CLASS_H
#define FRAME_CLASS_H


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "ShaderClass.h"

class Frame {
private:
	void Init();


public:
	// default constructor
	Frame();

	// laod shader
	void loadShader(Shader* shader);
	// laod a new shader
	void loadShader(const char* vertex, const char* fragment);

	void Draw();
	void DrawArray();

	void Delete();
private:
	static const float VertexPositions[36];

	// skybox textures, [right, left, top, bottom, front, back]
	GLuint vaoID, vboID;
	Shader* shader;
};

#endif