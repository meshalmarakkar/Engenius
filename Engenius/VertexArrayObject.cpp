#include "VertexArrayObject.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef _DEBUG
#define GL_ERROR_CHECK(x) gl_ClearError();\
		x;\
		ASSERT(gl_PrintError(#x, __FILE__, __LINE__)); //print function name, file its in, & line no
#else
#define GL_ERROR_CHECK(x) x; //dont need checks in release mode
#endif // DEBUG

static void gl_ClearError() {
	while (glGetError() != GL_NO_ERROR); //do in loop to get everything
}

static bool gl_PrintError(const char* functionName, const char* fileName, unsigned int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[GL_ERROR] : " << error << ", line : " << line << std::endl;
		std::cout << "           : " << functionName << " -in- " << fileName << std::endl;
		return false;
	}
	return true;
}

VertexArrayObject::VertexArrayObject() {
	GL_ERROR_CHECK(glGenVertexArrays(1, &VAO));
	GL_ERROR_CHECK(glBindVertexArray(VAO));
}

VertexArrayObject::~VertexArrayObject() {

}

void VertexArrayObject::bind() {
	GL_ERROR_CHECK(glBindVertexArray(VAO));
}

void VertexArrayObject::unbind() {
	GL_ERROR_CHECK(glBindVertexArray(0));
}

unsigned int VertexArrayObject::genBuffer() {
	GLuint buffer;
	GL_ERROR_CHECK(glGenBuffers(1, &buffer));
	VBOs.push_back(buffer);
	return VBOs.size() - 1;
}

unsigned int VertexArrayObject::genBuffer_andAddData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
	GLuint buffer;
	GL_ERROR_CHECK(glGenBuffers(1, &buffer));
	GL_ERROR_CHECK(glBindBuffer(target, buffer));
	GL_ERROR_CHECK(glBufferData(target, size, data, usage));

	VBOs.push_back(buffer);
	return VBOs.size() - 1;
}

void VertexArrayObject::addData(unsigned int bufferID, GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
	GL_ERROR_CHECK(glBindBuffer(target, VBOs[bufferID]));
	GL_ERROR_CHECK(glBufferData(target, size, data, usage));
}

void VertexArrayObject::enableVertexAttribArray(unsigned int attribID, int size, GLenum type, GLboolean normalised, GLsizei stride, size_t pointer) {
	GL_ERROR_CHECK(glEnableVertexAttribArray(attribID));
	GL_ERROR_CHECK(glVertexAttribPointer(attribID, size, type, normalised, stride, (GLvoid*)pointer));
}

void VertexArrayObject::enableVertexAttribArray_I(unsigned int attribID, int size, GLenum type, GLsizei stride, size_t pointer) {
	GL_ERROR_CHECK(glEnableVertexAttribArray(attribID));
	GL_ERROR_CHECK(glVertexAttribIPointer(attribID, size, type, stride, (GLvoid*)pointer)); //Int version
}

void VertexArrayObject::setDivisor(unsigned int bufferID, unsigned int divisor) {
	GL_ERROR_CHECK(glVertexAttribDivisor(bufferID, divisor));
}

void VertexArrayObject::setDivisor_multi(unsigned int bufferID_first, unsigned int bufferID_last, unsigned int divisor) {
	for (unsigned int i = bufferID_first; i < bufferID_last + 1; i++) {
		GL_ERROR_CHECK(glVertexAttribDivisor(i, divisor));
	}
}
