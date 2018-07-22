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
	
	static const GLenum gl_triangles = GL_TRIANGLES;
	static const GLenum gl_triangle_strip = GL_TRIANGLE_STRIP;
	static const GLenum gl_points = GL_POINTS;

	static const GLenum none = 0;
};

class VertexArrayObject
{
public:
	VertexArrayObject(const unsigned int& indicesCount, GLenum drawMode = GL_TRIANGLES);
	~VertexArrayObject();

	void bind();
	void unbind();

	unsigned int genBuffer();
	unsigned int genBuffer_andAddData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	void addData(const unsigned int& bufferID, GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	void updateData(const unsigned int& bufferID, GLenum target, GLsizeiptr size, const void* data);

	void enableVertexAttribArray(const unsigned int& attribID, const int& size, GLenum type, GLboolean normalised, GLsizei stride, size_t pointer);
	void enableVertexAttribArray_I(const unsigned int& attribID, const int& size, GLenum type, GLsizei stride, size_t pointer);

	void setDivisor(const unsigned int& bufferID, const unsigned int& divisor);
	void setDivisor_multi(const unsigned int& bufferID_first, const unsigned int& bufferID_last, const unsigned int& divisor);

	void setIndicesCount(const unsigned int& num);
	unsigned int getIndicesCount();

	void setRestartIndex(const unsigned int& num);
	unsigned int getRestartIndex();

	GLenum getDrawMode();

private:
	GLuint VAO;
	std::vector<GLuint> VBOs;

	unsigned int indicesCount;
	unsigned int restartIndex;

	GLenum drawMode;
};

#endif