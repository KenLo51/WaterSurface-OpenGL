#include "Texture.h"

const int Texture::maximumSolts = 16;
int Texture::currSolt = 0;
int Texture::reserveSoltsNum = 8;

// 2D Texture Class ///////////////////////////////////////////////////////////////////////

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

// create an opengl texture
void Texture::Init(const char* filename, int solt, int flipImage) {
	Texture::solt = solt;

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
	int solt = Texture::currSolt + 1;
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
	Texture::currSolt = Texture::reserveSoltsNum;
}


// do not use this
void Texture::ReserveSolt(int solt) {
	//Texture::reserveSoltsNum = solt;
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



// 3D Texture Class ///////////////////////////////////////////////////////////////////////
// default constructor
Texture3D::Texture3D() {
	Texture3D::ID = 0;
	Texture3D::solt = 0;
	Texture3D::data = NULL;
	Texture3D::width = -1;
	Texture3D::height = -1;
	Texture3D::depth = -1;
	Texture3D::nChannels = -1;
}

Texture3D::Texture3D(GLuint id) {
	Texture3D::ID = id;
	Texture3D::solt = 0;
	Texture3D::data = NULL;
	Texture3D::width = -1;
	Texture3D::height = -1;
	Texture3D::depth = -1;
	Texture3D::nChannels = -1;
}

// constructor an load images
Texture3D::Texture3D(const char* folder, int solt) {
	Texture3D::data = NULL;
	Texture3D::width = -1;
	Texture3D::height = -1;
	Texture3D::depth = -1;
	Texture3D::nChannels = -1;

	Texture3D::loadFiles(folder);
	Init(solt);
}

// load image file in folders with numbered image
bool Texture3D::loadFiles(const char* folder, int flipImage) {
	// finding image files
	std::vector<std::string> paths;
	for (const auto& entry : std::filesystem::directory_iterator(folder)) {
		// get file basedir, name, extension, full path
		std::string texPath = entry.path().string();
		size_t found_basedir = texPath.find_last_of("/\\");
		size_t found_extension = texPath.find_last_of(".");
		std::string name = texPath.substr(found_basedir + 1, found_extension - found_basedir - 1);
		std::string extension = texPath.substr(found_extension);

		// check if image
		if (extension != "png") return 1;

		// save image name
		paths.push_back(texPath);
	}

	// sort image files from low to heigh
	std::sort(paths.begin(), paths.end(), [](std::string a, std::string b) {
		if(a.length() != b.length()) return a < b;
		for (unsigned int i = 0; i < a.length(); i++)
			if (a[i] != b[i]) return a[i] < b[i];
		return false;
		});

	// load images
	stbi_set_flip_vertically_on_load(flipImage);
	// delete data if loaded previously
	if (Texture3D::data) {
		delete[] Texture::data;
		Texture3D::data = NULL;
		Texture3D::width = -1;
		Texture3D::height = -1;
		Texture3D::depth = paths.size();
		Texture3D::nChannels = -1;
	}

	for (unsigned int i = 0; i < paths.size(); i++) {
		std::string& texName = paths[i];

		// read new data
		int width, height, nChannels;
		unsigned char* newData = stbi_load(texName.c_str(), &width, &height, &nChannels, 0);
		// check if load fail
		if (!Texture::data) {
			std::cout << "Failed to load texture \"" << texName << "\"" << std::endl;
			exit(1);
		}

		// check if format can save to 3D texture
		if (((width != Texture3D::width) && (Texture3D::width != -1)) ||
			((height != Texture3D::height) && (Texture3D::height != -1)) ||
			((nChannels != Texture3D::nChannels) && (Texture3D::nChannels != -1))) {
			std::cout << "Failed to load texture. Different image format at different layer." << std::endl;
			exit(1);
		}

		// update texture parameter
		Texture3D::width = width;
		Texture3D::height = height;
		Texture3D::nChannels = nChannels;
		Texture3D::depth = Texture3D::depth + 1;

		// print status
		std::cout << "\rloading 3d texture " << i << "/" << paths.size() << ":" << texName.c_str() << "        ";

		
		if (Texture3D::data == NULL) // Create space to save texture
			Texture3D::data = new unsigned char[Texture3D::width * Texture3D::height * Texture3D::depth * Texture3D::nChannels];
		
		// copy 1 layear image to 3d texture data
		memcpy(Texture3D::data + Texture3D::width * Texture3D::height * i * Texture3D::nChannels,
			newData, 
			Texture3D::width * Texture3D::height * Texture3D::nChannels);

		// delete stbi data
		stbi_image_free(newData);
	}
	std::cout << std::endl;


	GLenum dataFormat = (Texture3D::nChannels == 3) ? GL_RGB :
		(Texture3D::nChannels == 4) ? GL_RGBA :
		(Texture3D::nChannels == 1) ? GL_RED :
		(Texture3D::nChannels == 2) ? GL_RG :
		GL_RGB;
	GLenum glFormat = (Texture3D::nChannels == 3) ? GL_RGB :
		(Texture3D::nChannels == 4) ? GL_RGBA :
		(Texture3D::nChannels == 1) ? GL_RED :
		(Texture3D::nChannels == 2) ? GL_RG :
		GL_RGB;

	// copy data to opengl
	glTexImage3D(GL_TEXTURE_3D, 0, glFormat, Texture3D::width, Texture3D::height, Texture3D::depth, 0, dataFormat, GL_UNSIGNED_BYTE, Texture3D::data);
	glGenerateMipmap(GL_TEXTURE_3D);
	
	return 0;
}

// create an opengl texture
void Texture3D::Init(int solt, int flipImage) {
	Texture3D::solt = solt;

	// create a texture
	glGenTextures(1, &(Texture3D::ID));

	// bind texture
	glActiveTexture(GL_TEXTURE0 + solt); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_3D, Texture3D::ID);


	// setting up texture
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//		border mode setting
	//			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//		Texture Filtering 
	//			2d or 3d texture; scaling up and scaling down; nearest or linear;
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//			Mipmaps. use low resolution textures when objects are far away.
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}
