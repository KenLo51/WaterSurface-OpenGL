#include "ShaderClass.h"

const char Shader::VERTEX = 1;
const char Shader::FRAGMENT = 2;
const char Shader::VERTEX_FRAGMENT = 3;

// read all contents from file
std::string Shader::get_file_contents(const char* filename) {
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

// Shader destructor
Shader::Shader(){
	Shader::ID = 0;
	Shader::vertexShader_ID = 0;
	Shader::fragmentShader_ID = 0;
}
Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	Shader::ID = glCreateProgram();

	//read shader file
	setVertexShaderSource(vertexShaderFile);
	setFragmentShaderSource(fragmentShaderFile);

	glLinkProgram(Shader::ID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}

// read and complile VertexShader file
void Shader::setVertexShaderSource(const char* vertexShaderFile) {
	// setting up Vertex Shader /////////////////////////////////////////////
	if (Shader::vertexShaders.count(vertexShaderFile) > 0) { // if hader has been loaded
		//std::cout << "vertex shader has been loaded" << std::endl;
		Shader::vertexShader_ID = Shader::vertexShaders[vertexShaderFile];
	}
	else { // load new vertex shader
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
		Shader::vertexShaders.insert(std::pair<std::string, GLuint>(std::string(vertexShaderFile), Shader::vertexShader_ID));
	}

	// add vertex shader to curr shader program
	glAttachShader(Shader::ID, Shader::vertexShader_ID);
}

// read and complile FragmentShader file
void Shader::setFragmentShaderSource(const char* fragmentShaderFile) {
	// setting up Fragment Shader /////////////////////////////////////////////
	if (Shader::fragmentShaders.count(fragmentShaderFile) > 0) {  // if hader has been loaded
		//std::cout << "fragment shader has been loaded" << std::endl;
		Shader::fragmentShader_ID = Shader::fragmentShaders[fragmentShaderFile];
	}
	else {  // load new fragment shader
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

		// add to cache
		Shader::fragmentShaders.insert(std::pair<std::string, GLuint>(std::string(fragmentShaderFile), Shader::fragmentShader_ID));
	}

	// add fragment shader to curr shader program
	glAttachShader(Shader::ID, Shader::fragmentShader_ID);
}

// update a pair of new shader
void Shader::setShadersSource(const char* vertexShaderFile, const char* fragmentShaderFile) {
	glDetachShader(Shader::ID, Shader::fragmentShader_ID);
	glDetachShader(Shader::ID, Shader::vertexShader_ID);
	setVertexShaderSource(vertexShaderFile);
	setFragmentShaderSource(fragmentShaderFile);
	glLinkProgram(Shader::ID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}

// delete all loaded shader
void Shader::clearShaderCache() {
	for (const std::pair<std::string, GLuint>& vertexShader : Shader::vertexShaders) {
		glDeleteShader(vertexShader.second);
	}
	Shader::vertexShaders.clear();
	for (const std::pair<std::string, GLuint>& vertexShader : Shader::fragmentShaders) {
		glDeleteShader(vertexShader.second);
	}
	Shader::fragmentShaders.clear();

	Shader::vertexShader_ID = 0;
	Shader::fragmentShader_ID = 0;
}

// delete a loaded shader
void Shader::removeShader(const char* fileName, const char type) {
	if (type == Shader::VERTEX || type == Shader::VERTEX_FRAGMENT) {
		if (Shader::vertexShader_ID == Shader::vertexShaders[fileName]) Shader::vertexShader_ID = 0;
		glDeleteShader(Shader::vertexShaders[fileName]);
		Shader::vertexShaders.erase(fileName);
	}
	if (type == Shader::FRAGMENT || type == Shader::VERTEX_FRAGMENT) {
		if (Shader::fragmentShader_ID == Shader::fragmentShaders[fileName]) Shader::fragmentShader_ID = 0;
		glDeleteShader(Shader::fragmentShaders[fileName]);
		Shader::fragmentShaders.erase(fileName);
	}
}

// utility uniform functions
void Shader::setUniform(const char* name, bool const value) {
	glUniform1i(glGetUniformLocation(Shader::ID, name), int(value));
}
void Shader::setUniform(const char* name, int const value) {
	glUniform1i(glGetUniformLocation(Shader::ID, name), value);
}
void Shader::setUniform(const char* name, unsigned int const value) {
	glUniform1ui(glGetUniformLocation(Shader::ID, name), value);
}
void Shader::setUniform(const char* name, float const value) {
	glUniform1f(glGetUniformLocation(Shader::ID, name), value);
}
void Shader::setUniform(const char* name, glm::vec2& value) {
	glUniform2fv(glGetUniformLocation(Shader::ID, name), 1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::vec3& value) {
	glUniform3fv(glGetUniformLocation(Shader::ID, name),1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::vec4& value) {
	glUniform4fv(glGetUniformLocation(Shader::ID, name), 1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::mat3& value) {
	glUniformMatrix3fv(glGetUniformLocation(Shader::ID, name), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(Shader::ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const int location, bool const value) {
	glUniform1i(location, int(value));
}
void Shader::setUniform(const GLint location, int const value) {
	glUniform1i(location, value);
}
void Shader::setUniform(const GLint location, unsigned int const value) {
	glUniform1ui(location, value);
}
void Shader::setUniform(const GLint location, float const value) {
	glUniform1f(location, value);
}
void Shader::setUniform(const GLint location, glm::vec2& value) {
	glUniform2fv(location, 1, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::vec3& value) {
	glUniform3fv(location, 1, glm::value_ptr(value));
}
void Shader::setUniform(const int location, glm::vec4& value) {
	glUniform4fv(location, 1, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::mat3& value) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::mat4& value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// use shader
void Shader::Activate() {
	glUseProgram(Shader::ID);
}

// delete shader
void Shader::Delete() {
	clearShaderCache();
	glDeleteProgram(Shader::ID);
	Shader::ID = 0;
}