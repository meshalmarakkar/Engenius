#include "InputManager.h"
#include "GameManager.h"

InputManager::InputManager(GameManager* gameManager, WindowManager*  windowManager, Camera* camera, EntityManager* entityManager, MousePicker* mousePicker, LightingManager* lightingManager, HUDManager* hudManager, AudioManager* audioManager, EditModeManager *editModeManager, CollisionManager* colManager, TerrainManager* terrainManager) :
	gameManager(gameManager), windowManager(windowManager), camera(camera), entityManager(entityManager), mousePicker(mousePicker), lightingManager(lightingManager), hudManager(hudManager), audioManager(audioManager), editModeManager(editModeManager), colManager(colManager), terrainManager(terrainManager) {
	warpMouse = true;
	SDL_ShowCursor(SDL_DISABLE);
	click_coolDown = 0.0f;
	can_click = true;
}

void InputManager::EditModeControls(const Uint8 *keys, const float dt_secs) {
	if (keys[SDL_SCANCODE_Q]) {
		WarpMouse();
	}

	if (keys[SDL_SCANCODE_B]) {
		colManager->writeBoundFile();
	}
	
	if (keys[SDL_SCANCODE_N]) {
		entityManager->writeFile();
	}

	if (keys[SDL_SCANCODE_T]) {
		entityManager->changeAnimation();
	}

	if (keys[SDL_SCANCODE_Y]) {
		lightingManager->toggleShadow();
	}

	if (keys[SDL_SCANCODE_X]) {
		editModeManager->toggleModes();
	}
	if (keys[SDL_SCANCODE_LCTRL]) {
		editModeManager->toggle_controlPlayer();
	}
	if (keys[SDL_SCANCODE_C]) {
		editModeManager->toggleViewMode();
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		editModeManager->switchModel(false);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		editModeManager->switchModel(true);
	}

	if (keys[SDL_SCANCODE_KP_ENTER]) {
		editModeManager->addFunction();
	}

	if (editModeManager->get_ifObjectsMode() || editModeManager->get_ifBoundingMode()) {
		if (keys[SDL_SCANCODE_KP_4]) {
			editModeManager->edit_SetScale(false);
		}
		if (keys[SDL_SCANCODE_KP_6]) {
			editModeManager->edit_SetScale(true);
		}

		if (keys[SDL_SCANCODE_UP]) {
			editModeManager->edit_ScaleRate(true);
		}
		if (keys[SDL_SCANCODE_DOWN]) {
			editModeManager->edit_ScaleRate(false);
		}

		if (keys[SDL_SCANCODE_KP_MINUS]) {
			editModeManager->edit_SetYOffset(false);
		}
		if (keys[SDL_SCANCODE_KP_PLUS]) {
			editModeManager->edit_SetYOffset(true);
		}
	}

	if (editModeManager->get_ifLightingMode()) {

		if (keys[SDL_SCANCODE_1])
			lightingManager->changeExposure(true);
		else if (keys[SDL_SCANCODE_2])
			lightingManager->changeExposure(false);
		if (keys[SDL_SCANCODE_3])
			lightingManager->toggleBloom();

		if (keys[SDL_SCANCODE_6]) {
			lightingManager->setPostProcessNum(1);
			std::cout << "blur" << std::endl;
		}
		else if (keys[SDL_SCANCODE_7]) {
			lightingManager->setPostProcessNum(2);
			std::cout << "retro" << std::endl;
		}
		else if (keys[SDL_SCANCODE_8]) {
			lightingManager->setPostProcessNum(3);
			std::cout << "intense" << std::endl;
		}
		else if (keys[SDL_SCANCODE_9]) {
			lightingManager->setPostProcessNum(4);
			std::cout << "edge" << std::endl;
		}
		else if (keys[SDL_SCANCODE_0]) {
			lightingManager->setPostProcessNum(0);
			std::cout << "normal" << std::endl;
		}
	}

	if (editModeManager->getViewMode() > 1) {
		if (editModeManager->get_ifControlPlayerMode()) {
			if (keys[SDL_SCANCODE_W])
				entityManager->getPlayer()->MoveForward();
			else if (keys[SDL_SCANCODE_S])
				entityManager->getPlayer()->MoveBackward();
			if (keys[SDL_SCANCODE_A])
				entityManager->getPlayer()->MoveLeft();
			else if (keys[SDL_SCANCODE_D])
				entityManager->getPlayer()->MoveRight();
		}
		else {
			if (keys[SDL_SCANCODE_W])
				camera->MoveForward(dt_secs);
			else if (keys[SDL_SCANCODE_S])
				camera->MoveBackward(dt_secs);
			if (keys[SDL_SCANCODE_A])
				camera->MoveLeft(dt_secs);
			else if (keys[SDL_SCANCODE_D])
				camera->MoveRight(dt_secs);
		}
	}

	if (editModeManager->get_ifObjectsMode()) {
		if (keys[SDL_SCANCODE_KP_7])
			editModeManager->edit_SetYaw90(false);
		if (keys[SDL_SCANCODE_KP_9])
			editModeManager->edit_SetYaw90(true);

		if (keys[SDL_SCANCODE_KP_8])
			editModeManager->edit_SetPitch90(false);
		if (keys[SDL_SCANCODE_KP_2])
			editModeManager->edit_SetPitch90(true);

		if (keys[SDL_SCANCODE_KP_1])
			editModeManager->edit_SetRoll90(false);
		if (keys[SDL_SCANCODE_KP_3])
			editModeManager->edit_SetRoll90(true);

		if (keys[SDL_SCANCODE_9])
			editModeManager->edit_GridNo(false);
		else if (keys[SDL_SCANCODE_0])
			editModeManager->edit_GridNo(true);
	}

	if (editModeManager->get_ifAudioMode()) {
		if (keys[SDL_SCANCODE_5]) {
			if (audioManager->IfPlaying(0))
				audioManager->Pause(0);
			else
				audioManager->Continue(0);
		}
	}

	if (editModeManager->get_ifBoundingMode()) {

		if (keys[SDL_SCANCODE_I])
			editModeManager->moveForward();
		else if (keys[SDL_SCANCODE_K])
			editModeManager->moveBackward();
		if (keys[SDL_SCANCODE_J])
			editModeManager->moveLeft();
		else if (keys[SDL_SCANCODE_L])
			editModeManager->moveRight();

		if (keys[SDL_SCANCODE_KP_7])
			editModeManager->edit_SetScaleX(true);
		else if (keys[SDL_SCANCODE_KP_1])
			editModeManager->edit_SetScaleX(false);
		if (keys[SDL_SCANCODE_KP_8])
			editModeManager->edit_SetScaleY(true);
		else if (keys[SDL_SCANCODE_KP_2])
			editModeManager->edit_SetScaleY(false);
		if (keys[SDL_SCANCODE_KP_9])
			editModeManager->edit_SetScaleZ(true);
		else if (keys[SDL_SCANCODE_KP_3])
			editModeManager->edit_SetScaleZ(false);

		if (keys[SDL_SCANCODE_KP_0]) {
			colManager->displayGridDetails();
		}
	}

}

bool InputManager::KeyboardControls(SDL_Window * window, const float dt_secs) {
	if (can_click == false) {
		click_coolDown += dt_secs;
		if (click_coolDown > MIN_WAIT_BETWEEN_CLICKS) {
			click_coolDown = 0.0f;
			can_click = true;
		}
	}

	while (SDL_PollEvent(&sdlEvent)) {
		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		if (sdlEvent.type == SDL_KEYDOWN)
		{
			if (keys[SDL_SCANCODE_ESCAPE])
				return false;
			if (gameManager->getIfPause() == false) {
				if (keys[SDL_SCANCODE_E]) {
					camera->ToggleZoom();
				}
				if (keys[SDL_SCANCODE_W]) {
					entityManager->getPlayer()->MoveForward();
				}
				else if (keys[SDL_SCANCODE_S]) {
					entityManager->getPlayer()->MoveBackward();
				}
				if (keys[SDL_SCANCODE_A]) {
					entityManager->getPlayer()->MoveLeft();
				}
				else if (keys[SDL_SCANCODE_D]) {
					entityManager->getPlayer()->MoveRight();
				}

				if (keys[SDL_SCANCODE_Z]) {
					editModeManager->toggleEditMode();
				}

				if (keys[SDL_SCANCODE_F]) {
					gameManager->toggleDeferredShading();
				}

				if (keys[SDL_SCANCODE_G]) {
					entityManager->shadowTest();
				}

				if (keys[SDL_SCANCODE_H]) {
					entityManager->increNum();
				}

			}
			else {
				if (keys[SDL_SCANCODE_1]) {
					gameManager->setPause(false);
				}
			}
			if (editModeManager->get_ifEditMode())
				EditModeControls(keys, dt_secs);
		}
	}

	MouseMovement(window);
	MouseControls();

	return true;
}

bool InputManager::MenubarSelection() {
	int SCREENHEIGHT = windowManager->getScreenHeight();
	bool clicked = false;
	if (hudManager->checkIfClicked(mouseX, mouseY, SCREENHEIGHT, "menu_objects")) {
		editModeManager->set_ObjectsMode();
		clicked = true;
	}
	else if (hudManager->checkIfClicked(mouseX, mouseY, SCREENHEIGHT, "menu_bounding")) {
		editModeManager->set_BoundingMode();
		clicked = true;
	}
	else if (hudManager->checkIfClicked(mouseX, mouseY, SCREENHEIGHT, "menu_lighting")) {
		editModeManager->set_LightingMode();
		clicked = true;
	}
	else if (hudManager->checkIfClicked(mouseX, mouseY, SCREENHEIGHT, "menu_audio")) {
		editModeManager->set_AudioMode();
		clicked = true;
	}

	if (hudManager->checkSubBarClicked(mouseX, mouseY, SCREENHEIGHT, "sub_add")) {
		editModeManager->toggleAddMode();
		clicked = true;
	}
	else if (hudManager->checkSubBarClicked(mouseX, mouseY, SCREENHEIGHT, "sub_move")) {
		editModeManager->toggleMoveMode();
		clicked = true;
	}
	else if (hudManager->checkSubBarClicked(mouseX, mouseY, SCREENHEIGHT, "sub_visualize")) {
		editModeManager->toggleVisualize();
		clicked = true;
	}

	if (editModeManager->get_ifLightingMode()) {
		if (hudManager->checkSubBarClicked(mouseX, mouseY, SCREENHEIGHT, "sub_increase")) {
			hudManager->swapComponent(false);
			clicked = true;
		}
		else if (hudManager->checkSubBarClicked(mouseX, mouseY, SCREENHEIGHT, "sub_decrease")) {
			hudManager->swapComponent(true);
			clicked = true;
		}
	}

	return clicked;
}

void InputManager::MouseControls() {
	if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			if (can_click) {
				can_click = false;
				gameManager->checkTrigger();
				if (MenubarSelection() == false && editModeManager->get_ifAddMode()) {
					editModeManager->addFunction();
				}
			}
		}
	}
}

void InputManager::MouseMovement(SDL_Window * window) {

	SDL_GetMouseState(&mouseX, &mouseY);

	if (warpMouse == true) {
		float yaw = camera->getYaw();
		float pitch = camera->getPitch();
		const int MidX = static_cast<int>(windowManager->getScreenWidth() / 2);
		const int MidY = static_cast<int>(windowManager->getScreenHeight() / 2);

		yaw += 0.1f*(mouseX - MidX);
		pitch += 0.1f*(MidY - mouseY); //opposite as y goes from bottom to up in coord

		const float PITCH_LOCK = 45.0f;// 4.0f;
		if (pitch > PITCH_LOCK)
			pitch = PITCH_LOCK;
		if (pitch < -PITCH_LOCK)
			pitch = -PITCH_LOCK;
		if (yaw < 0.0)
			yaw += 360.0;
		if (yaw > 360.0)
			yaw -= 360;

		camera->setYaw(yaw);
		camera->setPitch(pitch);

		SDL_WarpMouseInWindow(window, MidX, MidY);
	}
}

void InputManager::WarpMouse() {
	warpMouse = !warpMouse;
	if (warpMouse) {
		SDL_ShowCursor(SDL_DISABLE);
	}
	else {
		SDL_ShowCursor(SDL_ENABLE);
	}
}

bool InputManager::get_ifWarpMouse() {
	return warpMouse;
}