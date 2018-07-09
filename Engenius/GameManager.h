
//include guards!
#ifndef GAMEMANAGER
#define GAMEMANAGER

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
#include "Renderer.h"
#include "TextureLoader.h"

#include <iostream>
#include <math.h>
#include "SDL.h" 
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>
#include <string>
#include <time.h>

class GameManager {
public:
	GameManager();
	~GameManager();
	void init(WindowManager * windowManager);
	void update(float _dt_secs);
	void draw();
	bool ControlCheck(const float dt_secs);

private:
	void renderScene();
	void renderScene_GBuffer();

private:	
	unsigned int particle_program;
	ParticleManager* particleGenerator;
	LightingManager* lightingManager;
	Camera* camera;
	EntityManager * entityManager;
	HUDManager * hudManager;
	InputManager * inputManager;
	MousePicker * mousePicker;
	AudioManager * audioManager;
	EditModeManager * editModeManager;
	ShaderManager * shaderManager;
	WindowManager * windowManager;
	CollisionManager * colManager;
	TerrainManager * terrainManager;

	Renderer* renderer;

	unsigned int tex_pause;
	unsigned int tex_end;
	
	float dt_secs;

};

#endif