// 2022/11/26

#include "ShaderClass.h"


// ShaderBase Class ///////////////////////////////////////////////////////////
// default shader destructor
ShaderBase::ShaderBase() {
	ShaderBase::programID = 0;
}

// read all contents from file
std::string ShaderBase::get_file_contents(const char* filename) {
	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs.good()) {
		std::cout << "failed to opening \""<< filename << '\"' << std::endl;
		exit(1);
	}

	std::string fileContents;
	// getting file size
	ifs.seekg(0, std::ios_base::end);
	fileContents.resize(ifs.tellg());
	// reading file content
	ifs.seekg(0, std::ios_base::beg);
	ifs.read(&fileContents[0], fileContents.size());
	// close file
	ifs.close();

	return fileContents;
}

// use shader
void ShaderBase::Activate() {
	glUseProgram(ShaderBase::programID);
}

// utility uniform functions with given uniform name
void ShaderBase::setUniform(const char* name, bool const value) {
	glUniform1i(glGetUniformLocation(ShaderBase::programID, name), int(value)); }
void ShaderBase::setUniform(const char* name, int const value) {
	glUniform1i(glGetUniformLocation(ShaderBase::programID, name), value); }
void ShaderBase::setUniform(const char* name, unsigned int const value) {
	glUniform1ui(glGetUniformLocation(ShaderBase::programID, name), value); }
void ShaderBase::setUniform(const char* name, float const value) {
	glUniform1f(glGetUniformLocation(ShaderBase::programID, name), value); }
void ShaderBase::setUniform(const char* name, glm::vec2& value) {
	glUniform2fv(glGetUniformLocation(ShaderBase::programID, name), 1, glm::value_ptr(value)); }
void ShaderBase::setUniform(const char* name, glm::vec3& value) {
	glUniform3fv(glGetUniformLocation(ShaderBase::programID, name), 1, glm::value_ptr(value)); }
void ShaderBase::setUniform(const char* name, glm::vec4& value) {
	glUniform4fv(glGetUniformLocation(ShaderBase::programID, name), 1, glm::value_ptr(value)); }
void ShaderBase::setUniform(const char* name, glm::mat3& value) {
	glUniformMatrix3fv(glGetUniformLocation(ShaderBase::programID, name), 1, GL_FALSE, glm::value_ptr(value)); }
void ShaderBase::setUniform(const char* name, glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(ShaderBase::programID, name), 1, GL_FALSE, glm::value_ptr(value)); }

// utility uniform functions with given uniform location
void ShaderBase::setUniform(const int location, bool const value) {
	glUniform1i(location, int(value)); }
void ShaderBase::setUniform(const GLint location, int const value) {
	glUniform1i(location, value); }
void ShaderBase::setUniform(const GLint location, unsigned int const value) {
	glUniform1ui(location, value); }
void ShaderBase::setUniform(const GLint location, float const value) {
	glUniform1f(location, value); }
void ShaderBase::setUniform(const GLint location, glm::vec2& value) {
	glUniform2fv(location, 1, glm::value_ptr(value)); }
void ShaderBase::setUniform(const GLint location, glm::vec3& value) {
	glUniform3fv(location, 1, glm::value_ptr(value)); }
void ShaderBase::setUniform(const int location, glm::vec4& value) {
	glUniform4fv(location, 1, glm::value_ptr(value)); }
void ShaderBase::setUniform(const GLint location, glm::mat3& value) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
void ShaderBase::setUniform(const GLint location, glm::mat4& value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)); }

// Render Shader Class ///////////////////////////////////////////////////////////
// constant variables
const char Shader::VERTEX = 1;
const char Shader::FRAGMENT = 2;
const char Shader::VERTEX_FRAGMENT = 3;

// Shader destructor
Shader::Shader(){
	Shader::programID = 0;
	Shader::vertexShader_ID = 0;
	Shader::fragmentShader_ID = 0;
}
Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	Shader::programID = glCreateProgram();

	//read shader file
	setVertexShaderSource(vertexShaderFile);
	setFragmentShaderSource(fragmentShaderFile);

	glLinkProgram(Shader::programID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::programID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::programID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}

// read and complile VertexShader file
void Shader::setVertexShaderSource(const char* vertexShaderFile) {
	// setting up Vertex Shader
	if (Shader::vertexShaderName != vertexShaderFile){ // load new vertex shader
		// read shader file
		std::string vertexShaderSource = get_file_contents(vertexShaderFile);
		// create vertex shader
		Shader::vertexShader_ID = glCreateShader(GL_VERTEX_SHADER);

		// put shader code to opengl
		const char* vertexShaderSourcePtr = vertexShaderSource.c_str();
		glShaderSource(Shader::vertexShader_ID, 1, &vertexShaderSourcePtr, NULL);

		// compile Vertex Shader
		glCompileShader(Shader::vertexShader_ID);

		// check if compile success
		GLint success;
		glGetShaderiv(Shader::vertexShader_ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(Shader::vertexShader_ID, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(1);
		}

		// add to cache
		Shader::vertexShaderName = std::string(vertexShaderFile);
	}

	// add vertex shader to curr shader program
	glAttachShader(Shader::programID, Shader::vertexShader_ID);
}

// read and complile FragmentShader file
void Shader::setFragmentShaderSource(const char* fragmentShaderFile) {
	// setting up Fragment Shader
	if (Shader::fragmentShaderName != fragmentShaderFile) {  // load new fragment shader
		// read shader file
		std::string fragmentShaderSource = get_file_contents(fragmentShaderFile);
		// create fragment shader
		Shader::fragmentShader_ID = glCreateShader(GL_FRAGMENT_SHADER);

		// add shader source code to shader object
		const char* fragmentShaderSourcePtr = fragmentShaderSource.c_str();
		glShaderSource(Shader::fragmentShader_ID, 1, &fragmentShaderSourcePtr, NULL);

		// compile Vertex Shader
		glCompileShader(Shader::fragmentShader_ID);

		// check if compile success
		GLint success;
		glGetShaderiv(Shader::fragmentShader_ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(Shader::fragmentShader_ID, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(1);
		}

		Shader::fragmentShaderName = std::string(fragmentShaderFile);
	}

	// add fragment shader to curr shader program
	glAttachShader(Shader::programID, Shader::fragmentShader_ID);
}

// update a pair of new shader
void Shader::setShadersSource(const char* vertexShaderFile, const char* fragmentShaderFile) {
	glDetachShader(Shader::programID, Shader::fragmentShader_ID);
	glDetachShader(Shader::programID, Shader::vertexShader_ID);
	setVertexShaderSource(vertexShaderFile);
	setFragmentShaderSource(fragmentShaderFile);
	glLinkProgram(Shader::programID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::programID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::programID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}

// read and complile Shader file
void Shader::setShadersSource(const char chaderType, const char* ShaderFile) {
	GLuint currShaderID = 0;
	if (chaderType == Shader::VERTEX) {
		// if shader has been loaded
		if (Shader::vertexShaderName == std::string(ShaderFile)) return;
		// create shader
		currShaderID = glCreateShader(GL_VERTEX_SHADER);
	}
	if (chaderType == Shader::FRAGMENT) {
		// if shader has been loaded
		if (Shader::fragmentShaderName == std::string(ShaderFile)) return;
		// create shader
		currShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	}

	// if invalid chaderType
	if (currShaderID == NULL) return;

	// read shader file
	std::string shaderSource = get_file_contents(ShaderFile);

	// add shader source code to shader object
	const char* shaderSourcePtr = shaderSource.c_str();
	glShaderSource(currShaderID, 1, &shaderSourcePtr, NULL);

	// compile Shader
	glCompileShader(currShaderID);

	// check if compile success
	GLint success;
	glGetShaderiv(currShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// print error log
		char infoLog[512];
		glGetShaderInfoLog(currShaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::";
		if (chaderType == Shader::VERTEX) std::cout << "VERTEX::";
		if (chaderType == Shader::VERTEX) std::cout << "VERTEX::";
		std::cout << "COMPILATION_FAILED\n" << infoLog << std::endl;
		// delete
		glDeleteShader(currShaderID);
		//
		exit(1);
	}


	// save shader ID
	if (chaderType == Shader::VERTEX) {
		Shader::vertexShaderName = std::string(ShaderFile);
		Shader::vertexShader_ID = currShaderID;
	}
	if (chaderType == Shader::FRAGMENT) {
		// if shader has been loaded
		Shader::fragmentShaderName = std::string(ShaderFile);
		Shader::fragmentShader_ID = currShaderID;
	}
}

// delete shader
void Shader::Delete() {
	if (Shader::vertexShader_ID)
		glDeleteShader(Shader::vertexShader_ID);
	Shader::vertexShader_ID = 0;

	if (Shader::fragmentShader_ID)
		glDeleteShader(Shader::fragmentShader_ID);
	Shader::fragmentShader_ID = 0;

	if(Shader::programID)
		glDeleteProgram(Shader::programID);
	Shader::programID = 0;
}



// Compute Shader Class ///////////////////////////////////////////////////////
ComputeShader::ComputeShader() {
	ComputeShader::shaderID = 0;
	ComputeShader::programID = glCreateProgram();
}

// read and complile chader code
void ComputeShader::setComputeShaderSource(const char* shaderFile) {
	// setting up Fragment Shader
	if (ComputeShader::Name != shaderFile) {  // load new fragment shader
		// read shader file
		std::string fragmentShaderSource = Shader::get_file_contents(shaderFile);
		// create fragment shader
		ComputeShader::shaderID = glCreateShader(GL_COMPUTE_SHADER);

		// add shader source code to shader object
		const char* fragmentShaderSourcePtr = fragmentShaderSource.c_str();
		glShaderSource(ComputeShader::shaderID, 1, &fragmentShaderSourcePtr, NULL);

		// compile Vertex Shader
		glCompileShader(ComputeShader::shaderID);

		// check if compile success
		GLint success;
		glGetShaderiv(ComputeShader::shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(ComputeShader::shaderID, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(1);
		}

		ComputeShader::Name = std::string(shaderFile);
	}

	// add fragment shader to curr shader program
	glAttachShader(ComputeShader::programID, ComputeShader::shaderID);
	glLinkProgram(ComputeShader::programID);
}

void ComputeShader::Dispatch() {
}