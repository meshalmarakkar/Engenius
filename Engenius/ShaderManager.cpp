#include "ShaderManager.h"

/*
Based on rt3d.cpp from Real Time 3d graphics module
*/ 
ShaderManager::ShaderManager() {
	init(); 
}

// loadFile - loads text file from file fname as a char* 
// Allocates memory - so remember to delete after use
// size of file returned in fSize
char* ShaderManager::loadFile(const char *fname, GLint &fSize) {
	using namespace std;
	int size;
	char * memblock;

	// file read based on example in cplusplus.com tutorial
	// ios::ate opens file at the end
	ifstream file (fname, ios::in|ios::binary|ios::ate);
	if (file.is_open()) {
		size = (int) file.tellg(); // get location of file pointer i.e. file size
		fSize = (GLint) size;
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		std::cout << "file " << fname << " loaded" << std::endl;
	}
	else {
		std::cout << "Unable to open file " << fname << std::endl;
		fSize = 0;
		// should ideally set a flag or use exception handling
		// so that calling function can decide what to do now
		return nullptr;
	}
	return memblock;
}

// printShaderError
// Display (hopefully) useful error messages if shader fails to compile or link
void printShaderError(const GLint shader) {
	int maxLength = 0;
	int logLength = 0;
	GLchar *logMessage;

	// Find out how long the error message is
	if (!glIsShader(shader))
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	if (maxLength > 0) { // If message has some contents
		logMessage = new GLchar[maxLength];
		if (!glIsShader(shader))
			glGetProgramInfoLog(shader, maxLength, &logLength, logMessage);
		else
			glGetShaderInfoLog(shader,maxLength, &logLength, logMessage);
		std::cout << "Shader Info Log:" << std::endl << logMessage << std::endl;
		delete [] logMessage;
	}
	// should additionally check for OpenGL errors here
}

GLuint ShaderManager::initShaders(const char *vertFile, const char *fragFile) {
	GLuint p, f, v;

	char *vs,*fs;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);	

	// load shaders & get length of each
	GLint vlen;
	GLint flen;
	vs = loadFile(vertFile,vlen);
	fs = loadFile(fragFile,flen);
	
	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,&vlen);
	glShaderSource(f, 1, &ff,&flen);
	
	GLint compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << vertFile << " : Vertex shader not compiled." << std::endl;
		printShaderError(v);
	} 

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << fragFile << " : Fragment shader not compiled." << std::endl;
		printShaderError(f);
	} 
	
	p = glCreateProgram();
		
	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	//glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	//if (!compiled) {
	//	std::cout << fragFile << " : Linking Failed." << std::endl;
	//	printShaderError(f);
	//}
	glUseProgram(p);

	delete [] vs; // dont forget to free allocated memory
	delete [] fs; // we allocated this in the loadFile function...

	return p;
}

GLuint ShaderManager::initShaders(const char *vertFile, const char *fragFile, const char *geomFile) {
	GLuint p, f, v, g;

	char *vs, *fs, *gs;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	g = glCreateShader(GL_GEOMETRY_SHADER);

	// load shaders & get length of each
	GLint vlen;
	GLint flen;
	GLint glen;
	vs = loadFile(vertFile, vlen);
	fs = loadFile(fragFile, flen);
	gs = loadFile(geomFile, glen);

	const char * vv = vs;
	const char * ff = fs;
	const char * gg = gs;

	glShaderSource(v, 1, &vv, &vlen);
	glShaderSource(f, 1, &ff, &flen);
	glShaderSource(g, 1, &gg, &glen);

	GLint compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << vertFile << " : Vertex shader not compiled." << std::endl;
		printShaderError(v);
	}

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << fragFile << " : Fragment shader not compiled." << std::endl;
		printShaderError(f);
	}

	glCompileShader(g);
	glGetShaderiv(g, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << geomFile << " : Geometry shader not compiled." << std::endl;
		printShaderError(g);
	}

	p = glCreateProgram();

	glAttachShader(p, v);
	glAttachShader(p, f);
	glAttachShader(p, g);

	glLinkProgram(p);
	//glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	//if (!compiled) {
	//	std::cout << fragFile << " : Linking Failed." << std::endl;
	//	printShaderError(f);
	//}
	glUseProgram(p);

	delete[] vs; // dont forget to free allocated memory
	delete[] fs; // we allocated this in the loadFile function...
	delete[] gs;

	return p;
}

void ShaderManager::init() {
	mapped_model_program = initShaders("../Engenius/Shaders/mapped_model.vert", "../Engenius/Shaders/mapped_model.frag");
	model_program = initShaders("../Engenius/Shaders/model.vert", "../Engenius/Shaders/model.frag");
	terrain_program = initShaders("../Engenius/Shaders/terrain.vert", "../Engenius/Shaders/terrain.frag");
	terrain_mapped_program = initShaders("../Engenius/Shaders/terrain_mapped.vert", "../Engenius/Shaders/terrain_mapped.frag");
	animated_model_program = initShaders("../Engenius/Shaders/animated_model.vert", "../Engenius/Shaders/animated_model.frag");
	simple_program = initShaders("../Engenius/Shaders/simple.vert", "../Engenius/Shaders/simple.frag");
	gaussianBlur_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/blur.frag");
	postProcessing_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/postProcessing.frag");
	depthShader_program = initShaders("../Engenius/Shaders/shadowMap.vert", "../Engenius/Shaders/shadowMap.frag", "../Engenius/Shaders/shadowMap.gs");
	skybox_program = initShaders("../Engenius/Shaders/cubeMap.vert", "../Engenius/Shaders/cubeMap.frag");
	hud_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/HUD.frag");
	particle_program = initShaders("../Engenius/Shaders/particle.vert", "../Engenius/Shaders/particle.frag");
	depthMapRender_program = initShaders("../Engenius/Shaders/DepthMap.vert", "../Engenius/Shaders/DepthMap.frag");
	displayNormals_program = initShaders("../Engenius/Shaders/displayNormals.vert", "../Engenius/Shaders/displayNormals.frag", "../Engenius/Shaders/displayNormals.gs");
	firstPass_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/firstPass.frag");
}

GLuint ShaderManager::get_firstPass_program() {
	return firstPass_program;
}

GLuint ShaderManager::get_postProcessing_program() {
	return postProcessing_program;
}
GLuint ShaderManager::get_simple_program() {
	return simple_program;
}
GLuint ShaderManager::get_depthMapRender_program() {
	return depthMapRender_program;
}

GLuint ShaderManager::get_model_program() {
	return model_program;
}
GLuint ShaderManager::get_mapped_model_program() {
	return mapped_model_program;
}

GLuint ShaderManager::get_gaussianBlur_program() {
	return gaussianBlur_program;
}

GLuint ShaderManager::get_animated_model_program() {
	return animated_model_program;
}


GLuint ShaderManager::get_terrain_program() {
	return terrain_program;
}
GLuint ShaderManager::get_terrain_mapped_program() {
	return terrain_mapped_program;
}


GLuint ShaderManager::get_depthShader_program() {
	return depthShader_program;
}

GLuint ShaderManager::get_skybox_program() {
	return skybox_program;
}

GLuint ShaderManager::get_particle_program() {
	return particle_program;
}

GLuint ShaderManager::get_hud_program() {
	return hud_program;
}

GLuint ShaderManager::get_displayNormals_program() {
	return displayNormals_program;
}