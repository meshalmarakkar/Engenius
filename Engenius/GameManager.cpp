#include "GameManager.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

GameManager::GameManager() {
	dt_secs = 0.0f;
}

GameManager::~GameManager() {
	delete particleGenerator;
	delete lightingManager;
	delete camera;
	delete entityManager;
	delete hudManager;
	delete inputManager;
	delete mousePicker;
	delete audioManager;
	delete editModeManager;
	delete shaderManager;
	delete windowManager;
	delete colManager;
	delete terrainManager;
}

bool GameManager::ControlCheck(const float dt_secs) {
	this->dt_secs = dt_secs;
	return inputManager->KeyboardControls(windowManager->getWindow(), dt_secs);
}

void GameManager::init(WindowManager * windowManager) {
	this->windowManager = windowManager;
	SDL_GL_SwapWindow(windowManager->getWindow()); // swap buffers once

	camera = new Camera(windowManager->getScreenWidth(), windowManager->getScreenHeight());
	lightingManager = new LightingManager(camera->getCameraEye(), camera->getCameraAt());
	shaderManager = new ShaderManager(camera, lightingManager);

	hudManager = new HUDManager(windowManager, shaderManager->get_hud_program());
	audioManager = new AudioManager();
	
	particleGenerator = new ParticleManager(shaderManager->get_particle_program());

	//TODO: check what has to be stored and what has to be deleted from gameManager; like colManager for example
	colManager = new CollisionManager();

	terrainManager = new TerrainManager(lightingManager, shaderManager);

	tex_pause = TextureLoader::loadTexture("Menu.png", "../Engenius/Textures/");
	tex_end = TextureLoader::loadTexture("Exit.png", "../Engenius/Textures/");
	audioManager->startGame(camera->getCameraEye());
	entityManager = new EntityManager(camera, lightingManager, shaderManager, colManager, terrainManager);

	editModeManager = new EditModeManager(entityManager, colManager, particleGenerator, hudManager, lightingManager, audioManager, shaderManager);

	mousePicker = new MousePicker(windowManager, entityManager, terrainManager);

	inputManager = new InputManager(this, windowManager, camera, entityManager, mousePicker, lightingManager, hudManager, audioManager, editModeManager, colManager, terrainManager);

	renderer = new Renderer(windowManager, shaderManager);

	ifDeferred = false;
	pause = true;
	endGame = false;


	controlReset_1 = false;
	controlReset_2 = false;
	//	alarmStopped = false;
	previousTriggerNo = 0;


	renderGridNo[0] = 1;
	renderGridNo[1] = 0;

	pool_of_steps1[0] = glm::vec4(1.0f, 3.0f, 4.0f, 2.0f);
	pool_of_steps1[1] = glm::vec4(3.0f, 1.0f, 2.0f, 4.0f);
	pool_of_steps1[2] = glm::vec4(2.0f, 1.0f, 4.0f, 3.0f);
	pool_of_steps1[3] = glm::vec4(2.0f, 1.0f, 3.0f, 4.0f);

	pool_of_steps2[0] = glm::vec4(2.0f, 4.0f, 3.0f, 1.0f);
	pool_of_steps2[1] = glm::vec4(4.0f, 2.0f, 3.0f, 1.0f);
	pool_of_steps2[2] = glm::vec4(3.0f, 4.0f, 1.0f, 2.0f);
	pool_of_steps2[3] = glm::vec4(4.0f, 1.0f, 2.0f, 3.0f);

	/* initialize random seed: */
	srand(unsigned int(time(NULL)));

	/* generate secret number between 0 and 4: */
	puzzle_firstTime.steps = pool_of_steps1[rand() % 4];
	puzzle_secondTime.steps = pool_of_steps2[rand() % 4];

	std::cout << "1st: " << puzzle_firstTime.steps.x << puzzle_firstTime.steps.y << puzzle_firstTime.steps.z << puzzle_firstTime.steps.w << std::endl;
	std::cout << "1st: " << puzzle_secondTime.steps.x << puzzle_secondTime.steps.y << puzzle_secondTime.steps.z << puzzle_secondTime.steps.w << std::endl;
}

void GameManager::renderScene() {
	//skybox->renderSkybox(camera->getProjection(), camera->getView());

	entityManager->draw(dt_secs, NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);

	
	if (editModeManager->get_ifEditMode()) {
		Shader* program = shaderManager->getShaderProgram(Programs::simple);
		program->bind();
		program->uniformsToShader_RUNTIME();
		unsigned int shader = program->getShaderProgram();

		editModeManager->draw(shader);
	}

	//renderer->unbindTextures(0, lightingManager->getNumOfLights());

	particleGenerator->renderParticles(camera->getView(), camera->getProjection()); //render last to blend with all the objecs!!
}

void GameManager::renderScene_GBuffer() {
	entityManager->draw(dt_secs, NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);
	//entityManager->draw_GBuffer(NUM_EFFECTIVE_GRIDS, renderGridNo);
}

void GameManager::update(float _dt_secs) {
	int viewMode = editModeManager->getViewMode();
	
	camera->Update(entityManager->getPlayer()->getPos(), viewMode);

	const glm::vec3 cameraEye = camera->getCameraEye();

	if (editModeManager->get_ifEditMode()) {
		glm::vec3 currentTerrainPointOfMouse = mousePicker->TerrainPointOfMouse(cameraEye, camera->getView(), camera->getProjection());
		editModeManager->update(currentTerrainPointOfMouse, dt_secs);
	}
	colManager->update();
	entityManager->Update(camera, dt_secs);
	audioManager->updateListenerPosition(camera->getCameraEye());

	areaControl();

	//handPos = playerPosition + handPos;
	//lightSources.spotLight.position = cameraEye;//handPos;
	//lightSources.spotLight.direction = cameraAt;

	particleGenerator->Update(dt_secs, cameraEye);
	if (editModeManager->get_ifEditMode())
		hudManager->update(dt_secs);
}

bool done = false;
void GameManager::draw() {
	if (lightingManager->getIfShadow() == true && done ==false) {
		unsigned int shader = shaderManager->get_depthShader_program();
		glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera->getCameraEye()));

		for (unsigned int i = 0; i < lightingManager->getNumOfPointLights(); i++) {
			lightingManager->setUpShadowRender_Pointlights(shader, i); // render using light's point of view
		//	glUniform1f(glGetUniformLocation(shader, "far_plane"), camera->getFarPlane());
		//	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera->getCameraEye()));

			entityManager->shadow_draw(shader, NUM_EFFECTIVE_GRIDS, renderGridNo);
			done = true;
		}
	}

	if (ifDeferred) {
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//	renderer->disableBlend();
		//G-Buffer Render (DEFERRED SHADING P.1)
		renderer->setupFrame_Deferred();
		renderScene_GBuffer();

		//Deferred Shading (DEFERRED SHADING P.2)
		renderer->setupFrame_PostProcess();

		Shader* program = shaderManager->getShaderProgram(Programs::deferred_shading_mapped);
		program->bind();
		program->uniformsToShader_RUNTIME();
		unsigned int shader = program->getShaderProgram();
		renderer->toShader_Buffers(shader);

		//RENDER TO SCREEN WITH POST-PROCESSING
		renderer->disableDepthTest();
		renderer->draw_screenQuad();
	//	renderer->enableBlend();
	}
	else { 
		renderer->setupFrame_PostProcess();

		renderScene();
		renderer->disableDepthTest();
	}

	//glBindVertexArray(quadVAO);
	if (lightingManager->getIfBloom()) {
		renderer->SS_Bloom();
	}		

////	lightingManager->displayDepthMap(shaderManager->get_depthMapRender_program());
//
//	//DEFERRED RENDERING BLENDING PROBLEM!!!!!
//	if (!entityManager->getIfDef()) {
//		/*glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE);*/
//	}

	renderer->draw_finalDisplay(lightingManager->getExposure(), lightingManager->getIfBloom(), lightingManager->getPostProcessNum(), pause, tex_pause, endGame, tex_end);

	renderer->setBlendFunction(BlendOptions::gl_one_minus_src_alpha);

	//At end to see objects with transparency
	if (editModeManager->get_ifEditMode())
		hudManager->render();

	renderer->enableDepthTest();
	
	SDL_GL_SwapWindow(windowManager->getWindow()); // swap buffers
}

void GameManager::toggleDeferredShading() {
	ifDeferred = !ifDeferred;
}

void GameManager::setPause(const bool& newVal) {
	pause = newVal;
}

bool GameManager::getIfPause() {
	return pause;
}

void GameManager::areaControl() {
	unsigned int grid = colManager->getRelevantGrid(entityManager->getPlayer()->getPos(), entityManager->getPlayer()->getScale());
	if (grid == 1) {
		renderGridNo[0] = grid_entrance;
		renderGridNo[1] = grid_corridor;
	}
	else if (grid == 2) {
		renderGridNo[0] = grid_generator;
		renderGridNo[1] = grid_corridor;
	}
}

void GameManager::resetPuzzle(Puzzle *puzzle) {
	puzzle->gotFirst = false;
	puzzle->gotSecond = false;
	puzzle->gotThird = false;
}

void GameManager::checkStep(Puzzle *puzzle, const int& triggerNo) {
	if (puzzle->gotFirst == true) {
		if (puzzle->gotSecond == true) {
			if (puzzle->gotThird == true) {
				if (puzzle->steps[3] == triggerNo) {
					audioManager->playSFX_correct(camera->getCameraEye());
					puzzle->solved = true;
					audioManager->playSFX_correct(camera->getCameraEye());
					if (puzzle == &puzzle_firstTime)
						audioManager->playAlarm(camera->getCameraEye());
					else {
						audioManager->playSFX_gearSpinning(camera->getCameraEye());
						endGame = true;
					}
				}
				else if (triggerNo != previousTriggerNo) {
					audioManager->playSFX_wrong(camera->getCameraEye());
					resetPuzzle(puzzle);
					previousTriggerNo = 0;
				}
			}
			else {
				if (puzzle->steps[2] == triggerNo) {
					audioManager->playSFX_correct(camera->getCameraEye());
					puzzle->gotThird = true;
					previousTriggerNo = triggerNo;
				}
				else if (triggerNo != previousTriggerNo) {
					audioManager->playSFX_wrong(camera->getCameraEye());
					resetPuzzle(puzzle);
					previousTriggerNo = 0;
				}
			}
		}
		else {
			if (puzzle->steps[1] == triggerNo) {
				audioManager->playSFX_correct(camera->getCameraEye());
				puzzle->gotSecond = true;
				previousTriggerNo = triggerNo;
			}
			else if (triggerNo != previousTriggerNo) {
				audioManager->playSFX_wrong(camera->getCameraEye());
				resetPuzzle(puzzle);
				previousTriggerNo = 0;
			}
		}
	}
	else if (puzzle->steps[0] == triggerNo) {
		audioManager->playSFX_correct(camera->getCameraEye());
		puzzle->gotFirst = true;
		previousTriggerNo = triggerNo;
	}
	else if (triggerNo != previousTriggerNo) {
		audioManager->playSFX_wrong(camera->getCameraEye());
		resetPuzzle(puzzle);
		previousTriggerNo = 0;
	}
}

void GameManager::checkTrigger() {
	unsigned int grid = colManager->getRelevantGrid(entityManager->getPlayer()->getPos(), entityManager->getPlayer()->getScale());
	Puzzle *puzzle;
	if (puzzle_firstTime.solved == true && controlReset_2 == true) {
		puzzle = &puzzle_secondTime;
	}
	else if (controlReset_1 == true) {
		puzzle = &puzzle_firstTime;
	}

	if (controlReset_1 == true && puzzle->solved == false) {
		if (grid == 3) {
			checkStep(puzzle, 1);
		}
		else if (grid == 4) {
			checkStep(puzzle, 2);
		}
		else if (grid == 5) {
			checkStep(puzzle, 3);
		}
		else if (grid == 6) {
			checkStep(puzzle, 4);
		}
	}
	else if (grid == 7) {
		if (controlReset_1 == false) {
			controlReset_1 = true;
			audioManager->playSFX_correct(camera->getCameraEye());
		}
	}
	else if (grid == 8) {
		if (puzzle_firstTime.solved == true && controlReset_2 == false) {
			audioManager->playSFX_correct(camera->getCameraEye());
			audioManager->Stop("alarm");
		}
	}
	else if (grid == 9) {
		if (controlReset_2 == false) {
			audioManager->playSFX_correct(camera->getCameraEye());
			audioManager->Stop("alarm");
			controlReset_2 = true;
			audioManager->stopTense();
			lightingManager->setExposure(5.0f);
		}
	}
}
