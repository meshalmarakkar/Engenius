#ifndef SHADERMANAGER
#define SHADERMANAGER

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

class ShaderManager {
public:
	ShaderManager();

	void gl_UseProgram(unsigned int shader);
	void gl_ClearError();
	void gl_CheckError();

	void enableVertexAttribArray(unsigned int first, unsigned int last);
	void disableVertexAttribArray(unsigned int first, unsigned int last);
	void enableVertexAttribArray(unsigned int num);
	void disableVertexAttribArray(unsigned int num);

	unsigned int get_postProcessing_program();
	unsigned int get_simple_program();
	unsigned int get_model_program();
	unsigned int get_gaussianBlur_program();
	unsigned int get_mapped_model_program();
	unsigned int get_animated_model_program();
	unsigned int get_terrain_program();
	unsigned int get_terrain_mapped_program();
	unsigned int get_hud_program();
	unsigned int get_depthMapRender_program();
	unsigned int get_depthShader_program();
	unsigned int get_skybox_program();
	unsigned int get_particle_program();
	unsigned int get_displayNormals_program();
	unsigned int get_firstPass_program();
	unsigned int get_gBuffer_mapped_program();
	unsigned int get_deferredShading_mapped_program();
	unsigned int get_gBuffer_program();
	unsigned int get_deferredShading_program();
	unsigned int get_grass_program();

private:
	void init();
	char* loadFile(const char *fname, int &fSize);
	unsigned int initShaders(const char *vertFile, const char *fragFile);
	unsigned int initShaders(const char *vertFile, const char *fragFile, const char *geomFile);

private:
	unsigned int num_nextAvailableTex = 0;
	const float DEGREE_TO_RADIAN = 0.017453293f;
	const float PIE = 3.14159265359f;

	unsigned int postProcessing_program;
	unsigned int simple_program;
	unsigned int model_program;
	unsigned int gaussianBlur_program;
	unsigned int mapped_model_program;
	unsigned int animated_model_program;
	unsigned int terrain_program;
	unsigned int terrain_mapped_program;
	unsigned int hud_program;
	unsigned int depthMapRender_program;
	unsigned int skybox_program;
	unsigned int particle_program;
	unsigned int displayNormals_program;
	unsigned int firstPass_program;
	unsigned int gBuffer_mapped_program;
	unsigned int deferredShading_mapped_program;
	unsigned int gBuffer_program;
	unsigned int deferredShading_program;
	unsigned int grass_program;

	unsigned int depthShader_program;

	unsigned int currentShader;
};

#endif