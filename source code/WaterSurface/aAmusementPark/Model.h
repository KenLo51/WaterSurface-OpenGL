#pragma once
#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

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
#include <tiny_obj_loader.h>
//#include <TinyGLTF/tiny_gltf.h>

class Material;
class Model {
private:
	// functions for loading gltf-2.0
	//void LoadGltf_bindModelNodes(tinygltf::Model& model, tinygltf::Node& node);
	//void LoadGltf_bindMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);

public:
	// Model constructor
	Model();
	Model(const char* objFile);
	Model(std::vector<Mesh*> meshes);
	// Model destructor

	// load model from obj file
	void LoadObj(const char * objFile);
	// load model from gltf-2.0 file
	//void LoadGltf(const char* gltfFile);
	// load model by instance
	void Instance(Model& refModel);

	// update transform by position, scale, orientation, up
	void UpdateTransform();
	void UpdateTransform(glm::mat4& ref);
	void UpdateTransform(glm::vec3& position, glm::vec3& scale, glm::vec3& orientation, glm::vec3& up);

	// draw model
	void Draw();
	void Draw(Shader& shader);

	// delete model
	void Delete();

public:
	// for drawing
	glm::mat4 transform;

	std::vector<Mesh*> meshs;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;

	//
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 orientation;
	glm::vec3 up;
};


class Material{
// constants or static variables
public:
	static const int TYPE_DIFFUSE;
	static const int TYPE_AMBIENT;
	static const int TYPE_SPECULAR;
	static const int TYPE_NORMAL;

// private funstions
private:

// public funstions
public:
	Material();
	Material(std::map<std::string, std::string>& fileNameDict);

	//void LoadTextures(const char* diffName, const char* diffAmbi, const char* diffSpec);
	//void LoadTextures(std::map<std::string, std::string>& fileNameDict);
	void LoadTextures(int type, const char *fileName);

	void Use(Shader& shader);
	// delete textures
	void Delete();

// private variables
public:
	Texture* texDiffuse;
	Texture* texAmbient;
	Texture* texSpecular;
	Texture* texNormal;

// public variables
public:

};
#endif