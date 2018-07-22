
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
#include "Skybox.h"

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

	void toggleDeferredShading();
	void setPause(const bool& newVal);
	bool getIfPause();

	void checkTrigger();

private:
	void renderScene();
	void renderScene_GBuffer();

	struct Puzzle {
		glm::vec4 steps;
		bool gotFirst;
		bool gotSecond;
		bool gotThird;
		bool solved;

		Puzzle() {
			gotFirst = false;
			gotSecond = false;
			gotThird = false;
			solved = false;
		}
	};

	void areaControl();
	void resetPuzzle(Puzzle *puzzle);
	void checkStep(Puzzle *puzzle, const int& triggerNo);

private:	
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

	Skybox* skybox;

	unsigned int tex_pause;
	unsigned int tex_end;
	
	float dt_secs;

	bool ifDeferred;
	bool pause;
	bool endGame;

	const unsigned int grid_entrance = 0;
	const unsigned int grid_corridor = 1;
	const unsigned int grid_generator = 2;

	glm::vec4 pool_of_steps1[4];
	glm::vec4 pool_of_steps2[4];

	int previousTriggerNo;

	bool controlReset_1;
	bool controlReset_2;
	//bool alarmStopped;

	Puzzle puzzle_firstTime;
	Puzzle puzzle_secondTime;

#define NUM_EFFECTIVE_GRIDS 2
	unsigned int renderGridNo[NUM_EFFECTIVE_GRIDS];
};

#endif