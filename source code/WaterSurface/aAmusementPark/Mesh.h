#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

typedef struct VERTEX_TYPEDEF{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 texCoord;
	glm::vec4 tangent;

	bool operator ==(const VERTEX_TYPEDEF& a);
}Vertex;

class Mesh {
public:
//private:
	//static std::map <std::string, Texture> textures;
	GLuint vbo_ID, vao_ID, ebo_ID;

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	glm::mat4 transform;
public:
	// Mesh constructor
	Mesh();
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

	//
	void Load(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
	// draw mesh
	void Draw();
	// delete mesh
	void Delete();
};

#endif