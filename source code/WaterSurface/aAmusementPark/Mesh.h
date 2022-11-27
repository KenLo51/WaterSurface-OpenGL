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
public:
	// Mesh constructor
	Mesh();
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

	// build VAO VBO EBO for openGL by vertices and indices
	void init();

	// load vertices and indices
	void Load(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
	// draw mesh
	void Draw(GLenum mode = GL_TRIANGLES); // by indices array
	void DrawArray(GLenum mode = GL_TRIANGLES); // by vertices array

	// delete mesh
	void Delete();
};

#endif