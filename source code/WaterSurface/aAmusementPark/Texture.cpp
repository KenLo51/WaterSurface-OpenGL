#include "Texture.h"

const int Texture::maximumSolts = 16;
int Texture::currSolt = 0;
// used solt
std::set<int> Texture::usedSolt = std::set<int>();

// default constructor
Texture::Texture() {
	Texture::ID = 0;
	Texture::solt = 0;
	Texture::data = NULL;
	Texture::width = -1;
	Texture::height = -1;
	Texture::nChannels = -1;
}
// constructor with initialized texture id
Texture::Texture(GLuint id) {
	Texture::ID = id;
	Texture::solt = 0;
	Texture::data = NULL;
	Texture::width = -1;
	Texture::height = -1;
	Texture::nChannels = -1;
}
// constructor and read from file
Texture::Texture(const char* filename, int solt, int flipImage) {
	// setting up opengl texture parameter
	Init(filename, solt, flipImage);

	// load image from file
	loadFile(filename, flipImage);
}

// create an opengl texture and load file
void Texture::Init(const char* filename, int solt, int flipImage) {
	Texture::solt = solt;
	Texture::data = NULL;

	// create a texture
	glGenTextures(1, &(Texture::ID));

	// bind texture
	glActiveTexture(GL_TEXTURE0 + solt); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, Texture::ID);


	// setting up texture
	//		Texture Wrapping
	//			2d or 3d texture; s and t (and r)axis; repeat, mirror repeat, edge or border mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//		border mode setting
	//			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//		Texture Filtering 
	//			2d or 3d texture; scaling up and scaling down; nearest or linear;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//			Mipmaps. use low resolution textures when objects are far away.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}
// load image file and save in Texture::data, width, height, cChannels
bool Texture::loadFile(const char* filename, int flipImage) {
	stbi_set_flip_vertically_on_load(flipImage);

	// delete data if loaded previously
	if (Texture::data) {
		stbi_image_free(Texture::data);
		Texture::data = NULL;
		Texture::width = -1;
		Texture::height = -1;
		Texture::nChannels = -1;
	}

	// read new data
	Texture::data = stbi_load(filename, &(Texture::width), &(Texture::height), &(Texture::nChannels), 0);
	// check if load fail
	if (!Texture::data) {
		std::cout << "Failed to load texture \""<< filename << "\"" << std::endl;
		exit(1);
	}
	std::cout << "loading texture : " << filename << std::endl;
	std::cout << "  channel: " << Texture::nChannels << std::endl;
	GLenum dataFormat = (Texture::nChannels == 3) ? GL_RGB :
						(Texture::nChannels == 4) ? GL_RGBA :
						(Texture::nChannels == 1) ? GL_RED :
						(Texture::nChannels == 2) ? GL_RG :
						GL_RGB;
	GLenum glFormat = (Texture::nChannels == 3) ? GL_RGB :
		(Texture::nChannels == 4) ? GL_RGBA :
		(Texture::nChannels == 1) ? GL_RED :
		(Texture::nChannels == 2) ? GL_RG :
		GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, Texture::width, Texture::height, 0, dataFormat, GL_UNSIGNED_BYTE, Texture::data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

// bind texture with perives solt
void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + Texture::solt);
	glBindTexture(GL_TEXTURE_2D, ID);
	usedSolt.insert(Texture::solt);
}
// bind texture with given solt, -1 will find an unused solt and bind
void Texture::Bind(int solt) {
	if (solt < 0) {
		BindAnySolt();
		return;
	}

	Texture::solt = solt;
	Bind();
}
// find an empty solt and bind texture
void Texture::BindAnySolt() {
	int solt = Texture::currSolt;

	// search an unused solt
	while (usedSolt.count(solt) != 0) {
		solt = (solt + 1) % Texture::maximumSolts;
		if (Texture::currSolt == solt) break;
	}

	Texture::currSolt = solt;
	Texture::solt = solt;
	Bind();
}
// use shader with uniform
void Texture::Use(Shader& shader, const char* uniform) {
	shader.setUniform(uniform, Texture::solt);
}
// Bind and use texture in shader
void Texture::BindAndUse(Shader& shader, const char* uniform) {
	BindAnySolt();
	Use(shader, uniform);
}

// unbind texture
void Texture::UnBind() {
	glBindTexture(GL_TEXTURE_2D, 0);
	// clear used solt
	usedSolt.clear();
	Texture::currSolt = 0;
}

// delete texture
void Texture::Delete() {
	// delete image data
	if (Texture::data) 
		stbi_image_free(Texture::data);

	// delete texture
	if (Texture::ID != 0)
		glDeleteTextures(1, &(Texture::ID));

	Texture::ID = 0;
	Texture::solt = 0;
	Texture::data = NULL;
	Texture::width = -1;
	Texture::height = -1;
	Texture::nChannels = -1;
}