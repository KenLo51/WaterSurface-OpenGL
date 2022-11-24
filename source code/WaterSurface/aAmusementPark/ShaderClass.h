#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	static const char VERTEX;
	static const char FRAGMENT;
	static const char VERTEX_FRAGMENT;
private:
	// read all contents from file
	std::string get_file_contents(const char* filename);
private:
	// store loaded vertexShaders
	std::map <std::string, GLuint> vertexShaders;
	// store loaded fragmentShaders
	std::map <std::string, GLuint> fragmentShaders;
	// current used shader IDs
	GLuint vertexShader_ID, fragmentShader_ID;

public:
	// program shader ID
	GLuint ID;
	// Shader constructor
	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);

	// read and complile VertexShader file
	void setVertexShaderSource(const char* vertexShaderFile);
	// read and complile FragmentShader file
	void setFragmentShaderSource(const char* fragmentShaderFile);
	// update a pair of new shader
	void setShadersSource(const char* vertexShaderFile, const char* fragmentShaderFile);
	// delete all loaded shader
	void clearShaderCache();
	// delete a loaded shader
	void removeShader(const char* fileName, const char type);
	// utility uniform functions
	void setUniform(const char* name, bool const value);
	void setUniform(const char* name, int const value);
	void setUniform(const char* name, unsigned int const value);
	void setUniform(const char* name, float const value);
	void setUniform(const char* name, glm::vec2& value);
	void setUniform(const char* name, glm::vec3& value);
	void setUniform(const char* name, glm::vec4& value);
	void setUniform(const char* name, glm::mat3& value);
	void setUniform(const char* name, glm::mat4& value);

	void setUniform(const GLint location, bool const value);
	void setUniform(const GLint location, int const value);
	void setUniform(const GLint location, unsigned int const value);
	void setUniform(const GLint location, float const value);
	void setUniform(const GLint location, glm::vec2& value);
	void setUniform(const GLint location, glm::vec3& value);
	void setUniform(const GLint location, glm::vec4& value);
	void setUniform(const GLint location, glm::mat3& value);
	void setUniform(const GLint location, glm::mat4& value);

	// use shader
	void Activate();
	// delete shader
	void Delete();
};

#endif