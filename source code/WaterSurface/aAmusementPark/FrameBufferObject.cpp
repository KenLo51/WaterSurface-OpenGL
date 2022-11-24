#include "FrameBufferObject.h"

void FrameBuffer::init() {
	FrameBuffer::fboID = 0;
	FrameBuffer::texture = NULL;
}

FrameBuffer::FrameBuffer() {
	init();
}

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) {
	init();

	GLuint texID;
	glGenFramebuffers(1, &(FrameBuffer::fboID));
	glGenTextures(1, &texID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	FrameBuffer::texture = new Texture(texID);


	// attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::fboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameBuffer::texture->ID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::fboID);
}

void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}