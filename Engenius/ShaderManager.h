#ifndef SHADERMANAGER
#define SHADERMANAGER

#include <GL/glew.h>
#include <unordered_map>

#include "Camera.h"
#include "LightingManager.h"
#include "Shader.h"

namespace Programs {
	static const char* post_processing = "post_processing";
	static const char* simple = "simple";
	static const char* model = "model";
	static const char* gaussian_blur = "gaussian_blur";
	static const char* model_mapped = "model_mapped";
	static const char* model_animated = "model_animated";
	static const char* terrain = "terrain";
	static const char* terrain_mapped = "terrain_mapped";
	static const char* hud = "hud";
	static const char* skybox = "skybox";
	static const char* particle = "particle";
	static const char* godray_firstPass = "godray_firstPass";
	static const char* deferred_gBuffer = "deferred_gBuffer";
	static const char* deferred_shading = "deferred_shading";
	static const char* deferred_gBuffer_mapped = "deferred_gBuffer_mapped";
	static const char* deferred_shading_mapped = "deferred_shading_mapped";
	static const char* grass = "grass";
	static const char* shadow_depthShader = "shadow_depthShader";
	 
	//DEBUG
	static const char* display_normal = "display_normal";
	static const char* display_depthMap = "display_depthMap";
};

class ShaderManager {
public:
	ShaderManager(Camera* camera, LightingManager* lightingManager);
	~ShaderManager();

	void gl_UseProgram(unsigned int shader);
	void gl_ClearError();
	void gl_CheckError();

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

	Shader* getShaderProgram(const char* shaderName);

private:
	void init();
	char* loadFile(const char *fname, int &fSize);
	GLuint initShaders(const char *vertFile, const char *fragFile);
	GLuint initShaders(const char *vertFile, const char *fragFile, const char *geomFile);

	void shaderInit();

private:
	std::unordered_map<const char*, Shader*> shaders;

	//Shader * currentShader;
	
	Camera* camera;
	LightingManager* lightingManager;

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
	GLuint gBuffer_mapped_program;
	GLuint deferredShading_mapped_program;
	GLuint gBuffer_program;
	GLuint deferredShading_program;
	GLuint grass_program;
	GLuint depthShader_program;
	
	GLuint currentShader;
};

#endif