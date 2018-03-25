#ifndef SHADERMANAGER
#define SHADERMANAGER

#include <GL/glew.h>
#include <iostream>
#include <fstream>

class ShaderManager {
public:
	ShaderManager();

	GLuint get_postProcessing_program();
	GLuint get_simple_program();
	GLuint get_model_program();
	GLuint get_gaussianBlur_program();
	GLuint get_mapped_model_program();
	GLuint get_animated_model_program();
	GLuint get_terrain_program();
	GLuint get_terrain_mapped_program();
	GLuint get_hud_program();
	GLuint get_depthMapRender_program();
	GLuint get_depthShader_program();
	GLuint get_skybox_program();
	GLuint get_particle_program();
	GLuint get_displayNormals_program();
	GLuint get_firstPass_program();

private:
	void init();
	char* loadFile(const char *fname, GLint &fSize);
	GLuint initShaders(const char *vertFile, const char *fragFile);
	GLuint initShaders(const char *vertFile, const char *fragFile, const char *geomFile);

	GLuint postProcessing_program;
	GLuint simple_program;
	GLuint model_program;
	GLuint gaussianBlur_program;
	GLuint mapped_model_program;
	GLuint animated_model_program;
	GLuint terrain_program;
	GLuint terrain_mapped_program;
	GLuint hud_program;
	GLuint depthMapRender_program;
	GLuint skybox_program;
	GLuint particle_program;
	GLuint displayNormals_program;
	GLuint firstPass_program;

	GLuint depthShader_program;
};

#endif