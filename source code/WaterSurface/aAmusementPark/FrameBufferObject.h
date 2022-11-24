#pragma once

#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <sys/stat.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "ShaderClass.h"

class FrameBuffer {
	// constants or static variables
public:

	// private funstions
private:
	void init();

	// public funstions
public:
	FrameBuffer();
	FrameBuffer(unsigned int width, unsigned int height);

	void Bind();
	void Unbind();

	void Delete();

	// private variables
public:
	GLuint fboID;
	Texture* texture;

	// public variables
public:

};


#endif