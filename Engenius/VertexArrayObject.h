#ifndef VERTEXARRAYOBJECT
#define VERTEXARRAYOBJECT

#include <GL/glew.h>
#include <vector>
#include <iostream>

struct Buffer_Options {
	static const GLenum gl_array_buffer = GL_ARRAY_BUFFER;
	static const GLenum gl_element_array_buffer = GL_ELEMENT_ARRAY_BUFFER;
	static const GLenum gl_static_draw = GL_STATIC_DRAW;
	static const GLenum gl_stream_draw = GL_STREAM_DRAW;
	static const GLenum gl_dynamic_draw = GL_DYNAMIC_DRAW;
	static const GLenum gl_float = GL_FLOAT;
	static const GLenum gl_int = GL_INT;
	static const GLenum gl_byte = GL_BYTE;
	static const GLenum gl_false = GL_FALSE;
	static const GLenum gl_true = GL_TRUE;

	static const GLenum none = 0;
};

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind();
	void unbind();

	unsigned int genBuffer();
	unsigned int genBuffer_andAddData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	void addData(unsigned int bufferID, GLenum target, GLsizeiptr size, const void* data, GLenum usage);

	void enableVertexAttribArray(unsigned int attribID, int size, GLenum type, GLboolean normalised, GLsizei stride, size_t pointer);
	void enableVertexAttribArray_I(unsigned int attribID, int size, GLenum type, GLsizei stride, size_t pointer);

	void setDivisor(unsigned int bufferID, unsigned int divisor);
	void setDivisor_multi(unsigned int bufferID_first, unsigned int bufferID_last, unsigned int divisor);

private:
	GLuint VAO;
	std::vector<GLuint> VBOs;
};

#endif