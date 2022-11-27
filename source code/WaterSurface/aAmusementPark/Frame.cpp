#include "Frame.h"

// skybox vertex positions
const float Frame::VertexPositions[36] = {
	// positions          
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
};

void Frame::Init() {
	glGenVertexArrays(1, &(Frame::vaoID));
	glGenBuffers(1, &(Frame::vboID));

	glBindVertexArray(Frame::vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, Frame::vboID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Frame::VertexPositions), &(Frame::VertexPositions), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// default constructor
Frame::Frame() {
	Frame::shader = NULL;
	Frame::vaoID = 0;
	Frame::vboID = 0;
	Init();
}

// laod shader
void Frame::loadShader(Shader* shader) {
	Frame::shader = shader;
}
// laod a new shader
void Frame::loadShader(const char* vertex, const char* fragment) {
	Frame::shader = new Shader(vertex, fragment);
}


void Frame::Draw() {
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	// use shader
	Frame::shader->Activate();

	// draw 
	glBindVertexArray(Frame::vaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	//glDisable(GL_DEPTH_TEST);
}

void Frame::DrawArray() {
	// draw 
	glBindVertexArray(Frame::vaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Frame::Delete() {
	if (Frame::shader) {
		Frame::shader->Delete();
		delete Frame::shader;
	}
	Frame::shader = NULL;
}