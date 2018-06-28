#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include <GL/glew.h> 
#include <vector>
#include <iostream>

struct Tex_Options {
	static const GLenum gl_linear = GL_LINEAR;
	static const GLenum gl_nearest = GL_NEAREST;
	static const GLenum gl_clamp_to_edge = GL_CLAMP_TO_EDGE;
	static const GLenum gl_rgba = GL_RGBA;
	static const GLenum gl_rgb = GL_RGB;
	static const GLenum gl_rgb16f = GL_RGB16F;
	static const GLenum gl_rgb16 = GL_RGB16;
	static const GLenum gl_float = GL_FLOAT;
	static const GLenum gl_int = GL_INT;
	static const GLenum gl_unsigned_byte = GL_UNSIGNED_BYTE;

	static const GLenum none = 0;
};

class Framebuffer {
public:
	Framebuffer(unsigned int width, unsigned int height, bool create_rboDepth);
	~Framebuffer();

	void bind();
	void unbind();

	void edit_viewport(unsigned int width, unsigned int height);
	void clearBuffers();
	void clearScreen();

	GLuint getColourBuffer(unsigned int num);

	void createTexturesAndAttach(unsigned int num_texToAdd, unsigned int width, unsigned int height, GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT);
	void createSingleTexture(GLenum internalFormat, unsigned int width, unsigned int height, GLenum format, GLenum type, GLenum minFilter, GLenum magFilter);
	void attachDrawBuffers();

private:
	unsigned int framebuffer;
	std::vector<unsigned int> colourBuffers;
	std::vector<GLenum> attachments;
	unsigned int rboDepth;
};

#endif