#include "VertexArrayObject.h"
#include "ErrorCheck.h"

VertexArrayObject::VertexArrayObject(const unsigned int& indicesCount, GLenum drawMode) : indicesCount(indicesCount), drawMode(drawMode) {
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

void VertexArrayObject::addData(const unsigned int& bufferID, GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
	GL_ERROR_CHECK(glBindBuffer(target, VBOs[bufferID]));
	GL_ERROR_CHECK(glBufferData(target, size, data, usage));
}

void VertexArrayObject::updateData(const unsigned int& bufferID, GLenum target, GLsizeiptr size, const void* data) {
	GL_ERROR_CHECK(glBindBuffer(target, VBOs[bufferID]));
	GL_ERROR_CHECK(glBufferSubData(target, 0, size, data));
}

void VertexArrayObject::enableVertexAttribArray(const unsigned int& attribID, const int& size, GLenum type, GLboolean normalised, GLsizei stride, size_t pointer) {
	GL_ERROR_CHECK(glEnableVertexAttribArray(attribID));
	GL_ERROR_CHECK(glVertexAttribPointer(attribID, size, type, normalised, stride, (GLvoid*)pointer));
}

void VertexArrayObject::enableVertexAttribArray_I(const unsigned int& attribID, const int& size, GLenum type, GLsizei stride, size_t pointer) {
	GL_ERROR_CHECK(glEnableVertexAttribArray(attribID));
	GL_ERROR_CHECK(glVertexAttribIPointer(attribID, size, type, stride, (GLvoid*)pointer)); //Int version
}

void VertexArrayObject::setDivisor(const unsigned int& bufferID, const unsigned int& divisor) {
	GL_ERROR_CHECK(glVertexAttribDivisor(bufferID, divisor));
}

void VertexArrayObject::setDivisor_multi(const unsigned int& bufferID_first, const unsigned int& bufferID_last, const unsigned int& divisor) {
	for (unsigned int i = bufferID_first; i < bufferID_last + 1; i++) {
		GL_ERROR_CHECK(glVertexAttribDivisor(i, divisor));
	}
}

void VertexArrayObject::setIndicesCount(const unsigned int& num) {
	indicesCount = num;
}

unsigned int VertexArrayObject::getIndicesCount() {
	return indicesCount;
}

void VertexArrayObject::setRestartIndex(const unsigned int& num) {
	restartIndex = num;
}

unsigned int VertexArrayObject::getRestartIndex() {
	return restartIndex;
}

GLenum VertexArrayObject::getDrawMode() {
	return drawMode;
}