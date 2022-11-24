#pragma once
#ifndef ANIMATED_TEXTURE_CLASS_H
#define ANIMATED_TEXTURE_CLASS_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include "Texture.h"

class AnimatedTexture {
public:
	std::vector<Texture*> frames;
	std::vector<float> delayTime;
	unsigned int currFrame;
	float currFrameTime;
public:
	AnimatedTexture(const char* folder);
	void loadFiles(const char* folder);

	void BindAndUse(Shader& shader, const char* uniform, float deltaTime);
	void Delete();
};

#endif