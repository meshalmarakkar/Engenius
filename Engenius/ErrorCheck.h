#pragma once

#ifdef _DEBUG

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GL_ERROR_CHECK(x) gl_ClearError();\
		x;\
		ASSERT(gl_PrintError(#x, __FILE__, __LINE__)); //print function name, file its in, & line no

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

//template <typedef T>
//static bool printMessage(const T& message1, const T& message2) {
//	std::cout << message1 << message2 << std::endl;
//}
//template <typedef T>
//#define PRINT(const T& message1, const T& message2) printMessage(message1, message2);


#else //if not debug mode

#define GL_ERROR_CHECK(x) x; //dont need checks in release mode
#define PRINT(const T& message1, const T& message2);

#endif


