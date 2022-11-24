#include "Skybox.h"

// skybox vertex positions
const float Skybox::VertexPositions[108] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

void Skybox::Init() {
	glGenVertexArrays(1, &(Skybox::vaoID));
	glGenBuffers(1, &(Skybox::vboID));

	glBindVertexArray(Skybox::vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, Skybox::vboID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Skybox::VertexPositions), &(Skybox::VertexPositions), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// default constructor
Skybox::Skybox() {
	Skybox::shader = NULL;
	Skybox::textureID = 0;
	Skybox::vaoID = 0;
	Skybox::vboID = 0;
}
// constructor, find images in folder
Skybox::Skybox(const char* folder) {
	// finding skybox image files
	std::vector<std::string> paths;
	paths.resize(6);
	for (const auto& entry : std::filesystem::directory_iterator(folder)) {
		std::string texPath = entry.path().string();
		size_t found_basedir = texPath.find_last_of("/\\");
		size_t found_extension = texPath.find_last_of(".");
		std::string name = texPath.substr(found_basedir + 1, found_extension - found_basedir - 1);
		std::string extension = texPath.substr(found_extension);
		//printf("%s || %s\n", name.c_str(), extension.c_str());

		if (name == "right") paths[0] = texPath;
		else if (name == "left") paths[1] = texPath;
		else if (name == "top") paths[2] = texPath;
		else if (name == "bottom") paths[3] = texPath;
		else if (name == "front") paths[4] = texPath;
		else if (name == "back") paths[5] = texPath;
		else {
			printf("can not find skybox image\n");
			exit(0);
		}
	}

	// load images
	loadImages(paths[0].c_str(), paths[1].c_str(),
		paths[2].c_str(), paths[3].c_str(), 
		paths[4].c_str(), paths[5].c_str());
	Init();
}
// constructor use given paths
Skybox::Skybox(const char* rightFile, const char* leftFile,
	const char* topFile, const char* bottomFile,
	const char* backFile, const char* frontFile) {

	loadImages(rightFile, leftFile,
		topFile, bottomFile,
		frontFile, backFile);
	Init();
}

// load 6 skybox images
void Skybox::loadImages(const char* rightFile, const char* leftFile,
	const char* topFile, const char* bottomFile,
	const char* backFile, const char* frontFile) {
	stbi_set_flip_vertically_on_load(0);
	// create opengl texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// read image files
	int width, height, nrChannels;
	unsigned char* data;
	const char* textures_faces[] = { leftFile , rightFile , topFile , bottomFile , backFile , frontFile };
	for (unsigned int i = 0; i < 6; i++)
	{
		data = stbi_load(textures_faces[i], &width, &height, &nrChannels, 0);
		if (!data) {
			std::cout << "Failed to load skybox texture \"" << textures_faces[i] << "\"" << std::endl;
			exit(1);
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}

	// setting up texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

// laod shader
void Skybox::loadShader(Shader* shader) {
	Skybox::shader = shader;
}
// laod a new shader
void Skybox::loadShader(const char* vertex, const char* fragment) {
	Skybox::shader = new Shader(vertex, fragment);
}


void Skybox::Draw(glm::mat4 viewMat) {
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	// use shader
	Skybox::shader->Activate();

	// skybox texture
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, Skybox::textureID);
	Skybox::shader->setUniform("skybox", (int)textureID);
	// camera matrix
	viewMat = glm::mat4(glm::mat3(viewMat));
	Skybox::shader->setUniform("view", viewMat);

	// draw skybox
	glBindVertexArray(Skybox::vaoID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	//glDisable(GL_DEPTH_TEST);
}

void Skybox::Delete() {
	if (Skybox::textureID) glDeleteTextures(1, &textureID);
	Skybox::textureID = 0;
	if (Skybox::shader){
		Skybox::shader->Delete();
		delete Skybox::shader;
	}
	Skybox::shader = NULL;
}