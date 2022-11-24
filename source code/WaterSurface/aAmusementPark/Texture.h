#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#include <glad/glad.h>

#include <stb/stb_image.h>

#include "ShaderClass.h"

class Texture {
private:
	static const int maximumSolts;
	static int currSolt;
	static std::set<int> usedSolt;
public:
	GLuint ID;
	int solt;
	unsigned char* data;
	int width, height, nChannels;
private:
	// create an opengl texture and load file
	void Init(const char* filename, int solt, int flipImage = 1);
public:
	// default constructor
	Texture();
	// constructor with initialized texture id
	Texture(GLuint id);
	// constructor and read from file
	Texture(const char* filename, int solt = 0, int flipImage = 1);

	

	// load image file and save in Texture::data, width, height, cChannels
	bool loadFile(const char* filename, int flipImage = 1);

	// bind texture with perives solt
	void Bind();
	// bind texture with given solt
	void Bind(int solt);
	// find an empty solt and bind texture
	void BindAnySolt();
	// use shader with uniform
	void Use(Shader& shader, const char *uniform);
	// Bind and use texture in shader
	void BindAndUse(Shader& shader, const char* uniform);
	// unbind texture
	static void UnBind();
	// delete texture
	void Delete();
};

#endif