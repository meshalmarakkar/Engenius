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

	Shader* getShaderProgram(const char* shaderName);

private:
	void init();
	char* loadFile(const char *fname, int &fSize);
	GLuint initShaders(const char *vertFile, const char *fragFile);
	GLuint initShaders(const char *vertFile, const char *fragFile, const char *geomFile);

	void shaderInit();

private:
	std::unordered_map<const char*, Shader*> shaders;
	
	Camera* camera;
	LightingManager* lightingManager;
};

#endif