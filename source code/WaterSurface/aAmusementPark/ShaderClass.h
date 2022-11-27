// 2022/11/26

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderBase {
public:
	// read all contents from file
	static std::string get_file_contents(const char* filename);
public: // public functions
	// default shader destructor
	ShaderBase();

	// utility uniform functions with given uniform name
	void setUniform(const char* name, bool const value);
	void setUniform(const char* name, int const value);
	void setUniform(const char* name, unsigned int const value);
	void setUniform(const char* name, float const value);
	void setUniform(const char* name, glm::vec2& value);
	void setUniform(const char* name, glm::vec3& value);
	void setUniform(const char* name, glm::vec4& value);
	void setUniform(const char* name, glm::mat3& value);
	void setUniform(const char* name, glm::mat4& value);

	// utility uniform functions with given uniform location
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

private:
public:
	// program ID
	GLuint programID;
};


class Shader : public ShaderBase {
public:
	static const char VERTEX;
	static const char FRAGMENT;
	static const char VERTEX_FRAGMENT;
private:
	// current used shader IDs
	GLuint vertexShader_ID, fragmentShader_ID;

	std::string vertexShaderName;
	std::string fragmentShaderName;
public:
	// Shader constructor
	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);

	// read and complile VertexShader file
	void setVertexShaderSource(const char* vertexShaderFile);
	// read and complile FragmentShader file
	void setFragmentShaderSource(const char* fragmentShaderFile);
	// update a pair of new shader
	void setShadersSource(const char* vertexShaderFile, const char* fragmentShaderFile);
	// update a pair of new shader
	void setShadersSource(const char chaderType, const char* ShaderFile);

	// delete shader
	void Delete();
};


class ComputeShader : ShaderBase{
private:
	// current used shader IDs
	GLuint shaderID;
	// current used shader fileName
	std::string Name;
public:
	// Shader constructor
	ComputeShader();
	// Shader constructor with loading shader source
	ComputeShader(const char* shaderFile);
	// read and complile shader
	void setComputeShaderSource(const char* shaderFile);
	//
	void Dispatch();
	void Dispatch(unsigned int num_groups_x, unsigned int num_groups_y,	unsigned int num_groups_z);

	// delete shader
	void Delete();
};
#endif