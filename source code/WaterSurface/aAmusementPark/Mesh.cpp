#include "Mesh.h"


bool VERTEX_TYPEDEF::operator ==(const VERTEX_TYPEDEF& a) {
	for (unsigned int i = 0; i < sizeof(Vertex); i++) {
		if (((char*)&a)[i] != ((char*)this)[i]) return false;
	}
	return true;
}

// Mesh constructor
Mesh::Mesh() {
	Mesh::vao_ID = 0;
	Mesh::vbo_ID = 0;
	Mesh::ebo_ID = 0;
}
Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices) {
	Mesh::vao_ID = 0;
	Mesh::vbo_ID = 0;
	Mesh::ebo_ID = 0;

	Mesh::Load(vertices, indices);
}

void Mesh::Load(std::vector <Vertex>& vertices, std::vector <GLuint>& indices) {
	if (Mesh::vao_ID != 0) {
		Mesh::Delete();
	}

	Mesh::vertices = std::vector <Vertex>(vertices.begin(), vertices.end());
	Mesh::indices = std::vector <GLuint>(indices.begin(), indices.end());

	//printf("%u\n", Mesh::vertices.size());

	// generate buffers
	glGenVertexArrays(1, &(Mesh::vao_ID));
	glGenBuffers(1, &(Mesh::vbo_ID));
	glGenBuffers(1, &(Mesh::ebo_ID));

	glBindVertexArray(Mesh::vao_ID);
	
	// Vertex Buffer Object
	// bind VBO into VAO
	glBindBuffer(GL_ARRAY_BUFFER, Mesh::vbo_ID);
	//		copy vertex data to VBO
	glBufferData(GL_ARRAY_BUFFER, Mesh::vertices.size() * sizeof(Vertex), Mesh::vertices.data(), GL_STATIC_DRAW);
	
	// Element Buffer Object
	//		bind EBO into VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::ebo_ID);
	//		copy data to EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh::indices.size() * sizeof(GLuint), Mesh::indices.data(), GL_STATIC_DRAW);

	//		setting attributes & register VBO
	//		typedef struct {
	//			glm::vec3 position;
	//			glm::vec3 normal;
	//			glm::vec4 color;
	//			glm::vec2 texCoord;
	//		}Vertex;
	//		link position
	//		void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//		link normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	//		link color(RGBA)
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	//		link texCoord
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(3);
	//		link tangent
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);
	

	// unbind VAO
	// NOTICE: Unbind VAO before EBO and VBO
	glBindVertexArray(0);

	//offsetof(Vertex, normal);
	//		unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//		unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// draw mesh
void Mesh::Draw() {
	// use vao
	glBindVertexArray(Mesh::vao_ID);

	// draw by VBO
	//glDrawArrays(GL_TRIANGLES, 0, Mesh::indices.size());
	// draw by EBO
	glDrawElements(GL_TRIANGLES, Mesh::indices.size(), GL_UNSIGNED_INT, 0);

	// unbind vao
	glBindVertexArray(0);
}

// delete mesh
void Mesh::Delete() {
	glDeleteVertexArrays(1, &(Mesh::vao_ID));
	glDeleteBuffers(1, &(Mesh::vbo_ID));
	glDeleteBuffers(1, &(Mesh::ebo_ID));

	Mesh::vao_ID = 0;
	Mesh::vbo_ID = 0;
	Mesh::ebo_ID = 0;
}