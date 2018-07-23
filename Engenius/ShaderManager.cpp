#include "ShaderManager.h"
#include "Uniforms.h"

#include <iostream>
#include <fstream>

ShaderManager::ShaderManager(Camera* camera, LightingManager* lightingManager) : camera(camera), lightingManager(lightingManager) {
	init(); 
}

ShaderManager::~ShaderManager() {
	for (std::unordered_map<const char*, Shader*>::iterator it = shaders.begin(); it != shaders.end(); it++) {
		delete &it;
	}
}

// loadFile - loads text file from file fname as a char* 
// Allocates memory - so remember to delete after use
// size of file returned in fSize
char* ShaderManager::loadFile(const char *fname, int &fSize) {
	using namespace std;
	int size;
	char * memblock;

	// file read based on example in cplusplus.com tutorial
	// ios::ate opens file at the end
	ifstream file (fname, ios::in|ios::binary|ios::ate);
	if (file.is_open()) {
		size = (int) file.tellg(); // get location of file pointer i.e. file size
		fSize = (int) size;
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
void printShaderError(const int shader) {
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
	int vlen;
	int flen;
	vs = loadFile(vertFile,vlen);
	fs = loadFile(fragFile,flen);
	
	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,&vlen);
	glShaderSource(f, 1, &ff,&flen);
	
	int compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << vertFile << " : Shader not compiled : [VERTEX]" << std::endl;
		printShaderError(v);
	} 

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << fragFile << " : Shader not compiled : [FRAGMENT]" << std::endl;
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
	int vlen;
	int flen;
	int glen;
	vs = loadFile(vertFile, vlen);
	fs = loadFile(fragFile, flen);
	gs = loadFile(geomFile, glen);

	const char * vv = vs;
	const char * ff = fs;
	const char * gg = gs;

	glShaderSource(v, 1, &vv, &vlen);
	glShaderSource(f, 1, &ff, &flen);
	glShaderSource(g, 1, &gg, &glen);

	int compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << vertFile << " : Shader not compiled : [VERTEX]" << std::endl;
		printShaderError(v);
	} 

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << fragFile << " : Shader not compiled : [FRAGMENT]" << std::endl;
		printShaderError(f);
	} 

	glCompileShader(g);
	glGetShaderiv(g, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << geomFile << " : Shader not compiled : [GEOMETRY]" << std::endl;
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

void ShaderManager::gl_UseProgram(const GLuint& shader) {
	glUseProgram(shader);
}

void ShaderManager::gl_ClearError() {
	while (glGetError != GL_NO_ERROR);
}

void ShaderManager::gl_CheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL ERROR] : " << error << std::endl; //convert to hexadecimal before checking glew
	}
}

void ShaderManager::init() {
	GLuint mapped_model_program = initShaders("../Engenius/Shaders/mapped_model.vert", "../Engenius/Shaders/mapped_model.frag");
	GLuint model_program = initShaders("../Engenius/Shaders/model.vert", "../Engenius/Shaders/model.frag");
	GLuint terrain_program = initShaders("../Engenius/Shaders/terrain.vert", "../Engenius/Shaders/terrain.frag");
	GLuint terrain_mapped_program = initShaders("../Engenius/Shaders/terrain_mapped.vert", "../Engenius/Shaders/terrain_mapped.frag");
	GLuint animated_model_program = initShaders("../Engenius/Shaders/animated_model.vert", "../Engenius/Shaders/animated_model.frag");
	GLuint simple_program = initShaders("../Engenius/Shaders/simple.vert", "../Engenius/Shaders/simple.frag");
	GLuint gaussianBlur_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/blur.frag");
	GLuint postProcessing_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/postProcessing.frag");
	GLuint depthShader_program = initShaders("../Engenius/Shaders/shadowMap.vert", "../Engenius/Shaders/shadowMap.frag", "../Engenius/Shaders/shadowMap.gs");
	GLuint skybox_program = initShaders("../Engenius/Shaders/cubeMap.vert", "../Engenius/Shaders/cubeMap.frag");
	GLuint hud_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/HUD.frag");
	GLuint particle_program = initShaders("../Engenius/Shaders/particle.vert", "../Engenius/Shaders/particle.frag");
	GLuint depthMapRender_program = initShaders("../Engenius/Shaders/DepthMap.vert", "../Engenius/Shaders/DepthMap.frag");
	GLuint displayNormals_program = initShaders("../Engenius/Shaders/displayNormals.vert", "../Engenius/Shaders/displayNormals.frag", "../Engenius/Shaders/displayNormals.gs");
	GLuint firstPass_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/firstPass.frag");
	GLuint gBuffer_mapped_program = initShaders("../Engenius/Shaders/mapped_modelGBuffer.vert", "../Engenius/Shaders/mapped_modelGBuffer.frag");
	GLuint deferredShading_mapped_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/mapped_modelDeferredShading.frag");
	GLuint gBuffer_program = initShaders("../Engenius/Shaders/modelGBuffer.vert", "../Engenius/Shaders/modelGBuffer.frag");
	GLuint deferredShading_program = initShaders("../Engenius/Shaders/screenSpace.vert", "../Engenius/Shaders/modelDeferredShading.frag");
	GLuint grass_program = initShaders("../Engenius/Shaders/grass.vert", "../Engenius/Shaders/grass.frag", "../Engenius/Shaders/grass.gs");

	typedef std::pair<const char*, Shader*> pair;

	//MODEL LOADING
	shaders.insert(pair(Programs::simple, new Shader(simple_program)));
	shaders.insert(pair(Programs::skybox, new Shader(skybox_program)));
	shaders.insert(pair(Programs::model, new Shader(model_program)));
	shaders.insert(pair(Programs::model_mapped, new Shader(mapped_model_program)));
	shaders.insert(pair(Programs::model_animated, new Shader(animated_model_program)));
	shaders.insert(pair(Programs::terrain, new Shader(terrain_program)));
	shaders.insert(pair(Programs::terrain_mapped, new Shader(terrain_mapped_program)));
	shaders.insert(pair(Programs::grass, new Shader(grass_program)));
	shaders.insert(pair(Programs::hud, new Shader(hud_program)));
	shaders.insert(pair(Programs::particle, new Shader(particle_program)));

	//DEFERRED RENDERING
	shaders.insert(pair(Programs::deferred_gBuffer, new Shader(gBuffer_program)));
	shaders.insert(pair(Programs::deferred_gBuffer_mapped, new Shader(gBuffer_mapped_program)));
	shaders.insert(pair(Programs::deferred_shading, new Shader(deferredShading_program)));
	shaders.insert(pair(Programs::deferred_shading_mapped, new Shader(deferredShading_mapped_program)));

	//PASSES
	shaders.insert(pair(Programs::gaussian_blur, new Shader(gaussianBlur_program)));
	shaders.insert(pair(Programs::godray_firstPass, new Shader(firstPass_program)));
	shaders.insert(pair(Programs::shadow_depthShader, new Shader(depthShader_program)));
	shaders.insert(pair(Programs::post_processing, new Shader(postProcessing_program)));

	//DEBUG
	shaders.insert(pair(Programs::display_normal, new Shader(displayNormals_program)));
	shaders.insert(pair(Programs::display_depthMap, new Shader(depthMapRender_program)));

	shaderInit();
}

static void camera_requiredLoad(Camera* camera, Uniforms* uni) {
	uni->addUniform("projection", camera->getProjection_Pointer());
}

static void lighting_requiredLoad(LightingManager* light, Uniforms* uni) {
	light->lights_preloadShader(uni);
	uni->addUniform("far_plane", light->getShadowFarPlane());
}

static void commonRunTime_uniforms(Camera* cam, Uniforms* uni) {
	uni->addUniform("viewPos", cam->getCameraEye_Pointer());
	uni->addUniform("view", cam->getView_Pointer());
}

static void generalUniformInit(Camera* camera, LightingManager* lightingManager, std::unordered_map<const char*, Shader*>& shaders, const char* programName) {
	Uniforms* uni;
	uni = shaders.at(programName)->getInitUniforms();
	camera_requiredLoad(camera, uni);
	lighting_requiredLoad(lightingManager, uni);

	shaders.at(programName)->bind();
	shaders.at(programName)->uniformsToShader_INIT();

	uni = shaders.at(programName)->getRunTimeUniforms();
	commonRunTime_uniforms(camera, uni);
}

static void cameraUniformInit(Camera* camera, std::unordered_map<const char*, Shader*>& shaders, const char* programName) {
	Uniforms* uni;
	uni = shaders.at(programName)->getInitUniforms();
	camera_requiredLoad(camera, uni);
	shaders.at(programName)->bind();
	shaders.at(programName)->uniformsToShader_INIT();
	uni = shaders.at(programName)->getRunTimeUniforms();
	commonRunTime_uniforms(camera, uni);
}

void ShaderManager::shaderInit() {
	generalUniformInit(camera, lightingManager, shaders, Programs::terrain);
	generalUniformInit(camera, lightingManager, shaders, Programs::terrain_mapped);
	generalUniformInit(camera, lightingManager, shaders, Programs::grass);

	generalUniformInit(camera, lightingManager, shaders, Programs::model);
	generalUniformInit(camera, lightingManager, shaders, Programs::model_mapped);
	generalUniformInit(camera, lightingManager, shaders, Programs::model_animated);

	//DEFERRED G-BUFFER
	cameraUniformInit(camera, shaders, Programs::deferred_gBuffer_mapped);
	//DEFERRED SHADING
	const char* programName = Programs::deferred_shading_mapped;
	Uniforms* uni = shaders.at(programName)->getInitUniforms();
	uni->addUniform("far_plane", lightingManager->getShadowFarPlane());
	//uni->addUniform("far_plane", camera->getFarPlane_Pointer());
	lighting_requiredLoad(lightingManager, uni);
	shaders.at(programName)->bind();
	shaders.at(programName)->uniformsToShader_INIT();
	uni = shaders.at(programName)->getRunTimeUniforms();
	uni->addUniform("viewPos", camera->getCameraEye_Pointer());
	//commonRunTime_uniforms(camera, uni);
	//////////////////

	programName = Programs::simple;
	uni = shaders.at(programName)->getInitUniforms();
	uni->addUniform("projection", camera->getProjection_Pointer());
	shaders.at(programName)->bind();
	shaders.at(programName)->uniformsToShader_INIT();
	uni = shaders.at(programName)->getRunTimeUniforms();
	uni->addUniform("view", camera->getView_Pointer());
}

Shader* ShaderManager::getShaderProgram(const char* shaderName) {
	if (shaders.find(shaderName) == shaders.end())
	{
		std::cout << "[ERROR] : " << "cannot find shader" << std::endl;
		return nullptr;
	}
	return shaders.at(shaderName);
}