#include "Framebuffer.h"
#include "ErrorCheck.h"

#include <iostream>

Framebuffer::Framebuffer(const unsigned int& width, const unsigned int& height, const bool& create_rboDepth) {
	GL_ERROR_CHECK(glGenFramebuffers(1, &framebuffer));
	GL_ERROR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));

	if (create_rboDepth) {
		// create a depth buffer renderbuffer object
		GL_ERROR_CHECK(glGenRenderbuffers(1, &rboDepth));
		GL_ERROR_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
		GL_ERROR_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)width, (GLsizei)height));
		GL_ERROR_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	}
}

Framebuffer::~Framebuffer() {
	;
}

void Framebuffer::createTexturesAndAttach(const unsigned int& num_texToAdd, const unsigned int& width, const unsigned int& height, GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT)
{
	unsigned int num_attachments = attachments.size();
	for (unsigned int i = 0 + num_attachments; i < num_attachments + num_texToAdd; i++) {
		GLuint colourBuffer;
		GL_ERROR_CHECK(glGenTextures(1, &colourBuffer));

		GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, colourBuffer));
		GL_ERROR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_FLOAT, NULL));
		GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
		GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
		if (wrapS != Tex_Options::none)
			GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
		if (wrapT != Tex_Options::none)
			GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
		GL_ERROR_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colourBuffer, 0));

		colourBuffers.push_back(colourBuffer);
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	attachDrawBuffers();
}

//dont forgot to call "attachDrawBuffers" after
void Framebuffer::createSingleTexture(GLenum internalFormat, const unsigned int& width, const unsigned int& height, GLenum format, GLenum type, GLenum minFilter, GLenum magFilter)
{
	GLuint colourBuffer;
	GL_ERROR_CHECK(glGenTextures(1, &colourBuffer));
	GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, colourBuffer));
	GL_ERROR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)width, (GLsizei)height, 0, format, type, NULL));
	GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
	GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
	GL_ERROR_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachments.size(), GL_TEXTURE_2D, colourBuffer, 0));

	colourBuffers.push_back(colourBuffer);
	attachments.push_back(GL_COLOR_ATTACHMENT0 + attachments.size());
}

void Framebuffer::attachDrawBuffers() {
	GLenum* drawBuffers = new GLenum[attachments.size()];
	for (unsigned int i = 0; i < attachments.size(); i++) {
		drawBuffers[i] = attachments[i];
	}
	// tell OpenGL which color attachments of this framebuffer we'll use as defualt is just the first one
	GL_ERROR_CHECK(glDrawBuffers(attachments.size(), drawBuffers));
	delete drawBuffers;
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}

void Framebuffer::bind() {
	GL_ERROR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
}

void Framebuffer::unbind() {
	GL_ERROR_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::edit_viewport(const unsigned int& width, const unsigned int& height) {
	GL_ERROR_CHECK(glViewport(0, 0, (GLsizei)width, (GLsizei)height));
}
void Framebuffer::clearBuffers() {
	GL_ERROR_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void Framebuffer::clearScreen() {
	GL_ERROR_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

GLuint Framebuffer::getColourBuffer(const unsigned int& num) {
	return colourBuffers[num];
}