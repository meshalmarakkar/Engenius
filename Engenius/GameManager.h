
//include guards!
#ifndef GAMEMANAGER
#define GAMEMANAGER

#include "Common.h"
//#include "Skybox.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "HUDManager.h"
#include "ParticleManager.h"
#include "LightingManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "MousePicker.h"
#include "AudioManager.h"
#include "EditModeManager.h"
#include "CollisionManager.h"
#include "WindowManager.h"
#include "TerrainManager.h"

#include <iostream>
#include <math.h>
#include "SDL.h" 
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>
#include <string>
#include <time.h>
const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};
class GameManager {
public:
	GameManager();
	void init(WindowManager * windowManager);
	void update(float _dt_secs);
	void draw();
	bool ControlCheck(const float dt_secs);

private:	

	unsigned int quadVAO, quadVBO;

	void initOtherFBOs();
	void renderScene();

	GLuint particle_program;
	ParticleManager* particleGenerator;
	LightingManager* lightingManager;
	Camera* camera;
	EntityManager * entityManager;
//	Skybox * skybox;
	HUDManager * hudManager;
	InputManager * inputManager;
	MousePicker * mousePicker;
	AudioManager * audioManager;
	EditModeManager * editModeManager;
	ShaderManager * shaderManager;
	//CollisionManager * colManager;
	WindowManager * windowManager;

	unsigned int postProcessFBO;
	unsigned int colourBuffer[2];
	unsigned int rboDepth;
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	GLuint tex_pause;
	GLuint tex_end;
	
	float dt_secs;

};

#endif