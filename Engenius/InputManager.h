#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "EntityManager.h"
#include "Camera.h"
#include "SDL.h"
#include "MousePicker.h"
#include "LightingManager.h"
#include "HUDManager.h"
#include "AudioManager.h"
#include "EditModeManager.h"
#include "CollisionManager.h"
#include "WindowManager.h"
#include "TerrainManager.h"
//#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class GameManager;

class InputManager {
public:
	InputManager(GameManager* gameManager, WindowManager*  windowManager, Camera* camera, EntityManager* entityManager, MousePicker* mousePicker, LightingManager* lightingManager, HUDManager* hudManager, AudioManager* audioManager, EditModeManager *editModeManager, CollisionManager* colManager, TerrainManager* terrainManager);
	void WarpMouse();
	bool get_ifWarpMouse();
	bool KeyboardControls(SDL_Window * window, const float dt_secs);

private:
	void MouseMovement(SDL_Window * window);
	void MouseControls();
	void EditModeControls(const Uint8 *keys, const float dt_secs);
	bool MenubarSelection();

	float click_coolDown;
	bool can_click;
	const float MIN_WAIT_BETWEEN_CLICKS = 0.15f;
	SDL_Event sdlEvent;  // variable to detect SDL events
	int mouseX, mouseY;
	bool warpMouse;
	glm::vec3 cameraEye;
	WindowManager*  windowManager;
	CollisionManager* colManager;
	EntityManager* entityManager;
	MousePicker* mousePicker;
	LightingManager* lightingManager;
	HUDManager* hudManager;
	AudioManager* audioManager;
	Camera* camera;
	EditModeManager* editModeManager;
	TerrainManager* terrainManager;
	
	GameManager* gameManager;
};

#endif