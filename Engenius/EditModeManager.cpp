#include "EditModeManager.h"

EditModeManager::EditModeManager(EntityManager * entityManager, CollisionManager* colManager, ParticleManager* particleManager, HUDManager* hudManager, LightingManager* lightingManager, AudioManager* audioManager, ShaderManager* shaderManager) : 
	entityManager(entityManager), colManager(colManager), particleManager(particleManager), hudManager(hudManager), lightingManager(lightingManager), audioManager(audioManager), shaderManager(shaderManager){
	init();
}

void EditModeManager::init() {
	edit_ModelIterator = entityManager->getEnviModel_Begin();
	edit_ObjectIterator = entityManager->getEnviObject_Begin();
	edit_HeirIterator = colManager->getHeirBegin();
	edit_RigidIterator = colManager->getRigidBegin();
	edit_boundLocation = std::pair<unsigned int, unsigned int>(0, 0);
	
	viewMode = 0;

	indexOfSelectedObject = 4;
	editMode = false;
	numMode = 0;
	scaleRate = 0.1f;
	edit_yOffset = 0.0f;

	gridNo = 0;

	mode_bounding = new Mode();
	mode_lights = new Mode();
	mode_audio = new Mode();
	mode_objects = new Mode();
	mode_selected = mode_objects;

	particleManager->insertFire(1, (*edit_ObjectIterator)->getPos() + glm::vec3(0.0f, 5.0f, 0.0f), true);

	bool_controlPlayer = false;
}

void EditModeManager::update( const glm::vec3 currentTerrainPointOfMouse, float dt_secs) {
	//if (mode_selected == mode_objects) {
	//	if (mode_selected->moveMode) {
	//		(*edit_ObjectIterator)->setPos(currentTerrainPointOfMouse);
	//		glm::mat4 model;
	//		Common::createModelMatrix(model, currentTerrainPointOfMouse, (*edit_ObjectIterator)->getYaw(), (*edit_ObjectIterator)->getScale());
	//		(*edit_ObjectIterator)->setModelMatrix(model);

	//		//entityManager->setEntityPos(currentTerrainPointOfMouse, indexOfSelectedObject);
	//	}
	//}

	if (mode_selected->addMode)
		edit_SetPos(currentTerrainPointOfMouse);
}

void EditModeManager::draw(unsigned int shader) {
	if (mode_selected == mode_bounding) {
		glm::mat4 edit_ModelMatrix;
		glm::vec3 scale(mode_selected->edit_scale.x, mode_selected->edit_scale.y, mode_selected->edit_scale.z);
		Common::createModelMatrix(edit_ModelMatrix, mode_selected->edit_pos, mode_selected->edit_yaw, scale);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		colManager->renderBoundingBoxes(shader, mode_selected->addMode ? true : false, mode_selected->visualize ? true : false, edit_ModelMatrix);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}
	else if (mode_selected == mode_objects) {
		if (mode_selected->addMode) {
			glm::mat4 edit_ModelMatrix;
			Common::createModelMatrix(edit_ModelMatrix, mode_selected->edit_pos, mode_selected->edit_scale, glm::vec3(mode_selected->edit_pitch, mode_selected->edit_yaw, mode_selected->edit_roll));
			if (mode_selected->visualize) {
				edit_ModelIterator->second->Draw(shader, edit_ModelMatrix);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				edit_ModelIterator->second->Draw(shader, edit_ModelMatrix);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		else if (mode_selected->moveMode) {
			particleManager->moveParticleManually((*edit_ObjectIterator)->getPos() + glm::vec3(0.0f, 5.0f, 0.0f));
			if (mode_selected->visualize) {
				(*edit_ObjectIterator)->getModel()->Draw(shader, (*edit_ObjectIterator)->getModelMatrix());
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				(*edit_ObjectIterator)->getModel()->Draw(shader, (*edit_ObjectIterator)->getModelMatrix());
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
	}
	else if (mode_selected == mode_lights) {

	}
}

////MODES////
void EditModeManager::resetCurrentMode() {
	mode_selected->addMode = false;
	mode_selected->moveMode = false;
	mode_selected->visualize = false;
}
void EditModeManager::toggleModes() {
	numMode++;
	if (numMode > 3)
		numMode = 0;

	if (numMode == 0) { set_ObjectsMode(); }
	else if (numMode == 1) { set_BoundingMode(); }
	else if (numMode == 2) { set_LightingMode(); }
	else if (numMode == 3) { set_AudioMode(); }
}
void EditModeManager::set_ObjectsMode() {
	resetCurrentMode();
	mode_selected = mode_objects;
	hudManager->displaySelected("menu_objects");
}
void EditModeManager::set_BoundingMode() {
	resetCurrentMode();
	mode_selected = mode_bounding;
	hudManager->displaySelected("menu_bounding");
}
void EditModeManager::set_LightingMode() {
	resetCurrentMode();
	mode_selected = mode_lights;
	hudManager->displaySelected("menu_lighting");
}
void EditModeManager::set_AudioMode() {
	resetCurrentMode();
	mode_selected = mode_audio;
	hudManager->displaySelected("menu_audio");
}

bool EditModeManager::get_ifBoundingMode() { return (mode_selected == mode_bounding); }
bool EditModeManager::get_ifObjectsMode() { return (mode_selected == mode_objects); }
bool EditModeManager::get_ifLightingMode() { return (mode_selected == mode_lights); }
bool EditModeManager::get_ifAudioMode() { return (mode_selected == mode_audio); }

////OPTIONS////
void EditModeManager::toggle_controlPlayer() { bool_controlPlayer = !bool_controlPlayer; }
void EditModeManager::toggleEditMode() { editMode = !editMode; }
void EditModeManager::toggleAddMode() {
	mode_selected->addMode = !mode_selected->addMode;
	if (mode_selected->moveMode == true && mode_selected->addMode == true)
		mode_selected->moveMode = false;
}
void EditModeManager::toggleMoveMode() { 
	mode_selected->moveMode = !mode_selected->moveMode;
	if (mode_selected->moveMode == true && mode_selected->addMode == true)
		mode_selected->addMode = false;
}
void EditModeManager::toggleVisualize() { 
	mode_selected->visualize = !mode_selected->visualize;
}

bool EditModeManager::get_ifControlPlayerMode() { return bool_controlPlayer; }
bool EditModeManager::get_ifEditMode() { return editMode; }
bool EditModeManager::get_ifAddMode() {	return mode_selected->addMode; }
bool EditModeManager::get_ifMoveMode() { return mode_selected->moveMode; }
bool EditModeManager::get_ifVisualize() { return mode_selected->visualize; }
int EditModeManager::getViewMode() {
	return viewMode;
}
void EditModeManager::toggleViewMode() {
	viewMode++;
	if (viewMode > 2) //2 as not 0 or 1 is free cam movement
		viewMode = 0;
}

void EditModeManager::switchModel(bool next) {
	if (mode_selected == mode_objects) {
		if (mode_selected->addMode) {
			if (next) {
				edit_ModelIterator++;
				if (edit_ModelIterator == entityManager->getEnviModel_End())
					edit_ModelIterator = entityManager->getEnviModel_Begin();
			}
			else {
				if (edit_ModelIterator == entityManager->getEnviModel_Begin())
					edit_ModelIterator = entityManager->getEnviModel_End();
				edit_ModelIterator--; // minus anyway as end is after last object
			}
		}
		else if (mode_selected->moveMode) {
			if (next) {
				edit_ObjectIterator++;
				if (edit_ObjectIterator == entityManager->getEnviObject_End())
					edit_ObjectIterator = entityManager->getEnviObject_Begin();
			}
			else {
				if (edit_ObjectIterator == entityManager->getEnviObject_Begin())
					edit_ObjectIterator = entityManager->getEnviObject_End();
				edit_ObjectIterator--; // minus anyway as end is after last object
			}
		}
	}

	if (mode_selected == mode_bounding) {
		if (mode_selected->moveMode) {
			if (next) {
				if (mode_selected->visualize) {
					edit_RigidIterator++;
					if (edit_RigidIterator == colManager->getRigidEnd())
						edit_RigidIterator = colManager->getRigidBegin();
				}
				else {
					edit_HeirIterator++;
					if (edit_HeirIterator == colManager->getHeirEnd())
						edit_HeirIterator = colManager->getHeirBegin();
				}
			}
			else {
				if (mode_selected->visualize) {
					if (edit_RigidIterator == colManager->getRigidBegin())
						edit_RigidIterator = colManager->getRigidEnd();
					edit_RigidIterator--; // minus anyway as end is after last object
				}
				else {
					if (edit_HeirIterator == colManager->getHeirBegin())
						edit_HeirIterator = colManager->getHeirEnd();
					edit_HeirIterator--; // minus anyway as end is after last object
				}
			}
		}
	}

	if (mode_selected == mode_lights) {
		if (mode_selected->addMode) {
			;
		}
		if (mode_selected->moveMode) {
			//switchLightTypes? like pointlight and spotlight;
		}
	}
}

////EDIT_DISPLAYOBJECT////
void EditModeManager::edit_SetPos(glm::vec3 pos) { 
	pos.y += edit_yOffset;
	mode_selected->edit_pos = pos; 
}
void EditModeManager::edit_SetScale(bool increase) {
	increase ? mode_selected->edit_scale += glm::vec3(scaleRate) : mode_selected->edit_scale -= glm::vec3(scaleRate);
	std::cout << "scale: " << mode_selected->edit_scale.x << ", " << mode_selected->edit_scale.y << ", " << mode_selected->edit_scale.z << std::endl;
}
void EditModeManager::edit_SetScaleX(bool increase) {
	increase ? mode_selected->edit_scale.x += scaleRate : mode_selected->edit_scale.x -= scaleRate;
}
void EditModeManager::edit_SetScaleY(bool increase) {
	increase ? mode_selected->edit_scale.y += scaleRate : mode_selected->edit_scale.y -= scaleRate;
}
void EditModeManager::edit_SetScaleZ(bool increase) {
	increase ? mode_selected->edit_scale.z += scaleRate : mode_selected->edit_scale.z -= scaleRate;
}
void EditModeManager::edit_ScaleRate(bool increase) {
	increase ? scaleRate += 0.0025f : scaleRate -= 0.0025f;
	std::cout << "scaleRate: " << scaleRate << std::endl;
}
void EditModeManager::edit_SetYaw(bool increase) {
	if (mode_selected == mode_objects)
		increase ? mode_selected->edit_yaw += 5.0f : mode_selected->edit_yaw -= 5.0f;
}

void EditModeManager::edit_SetPitch90(bool increase) {
	if (mode_selected == mode_objects)
		increase ? mode_selected->edit_pitch += 90.0f : mode_selected->edit_pitch -= 90.0f;
}
void EditModeManager::edit_SetYaw90(bool increase) {
	if (mode_selected == mode_objects)
		increase ? mode_selected->edit_yaw += 90.0f : mode_selected->edit_yaw -= 90.0f;
}
void EditModeManager::edit_SetRoll90(bool increase) {
	if (mode_selected == mode_objects)
		increase ? mode_selected->edit_roll += 90.0f : mode_selected->edit_roll -= 90.0f;
}

void EditModeManager::edit_SetYOffset(bool increase) {
	increase ? edit_yOffset += scaleRate : edit_yOffset -= scaleRate;
}

void EditModeManager::edit_GridNo(bool increase) {
	if (increase) {
		if (gridNo == 2)
			gridNo = 0;
		else
			gridNo++;
	}
	else {
		if (gridNo == 0)
			gridNo = 2;
		else
			gridNo--;
	}
	std::cout << "gridNo: " << gridNo << std::endl;
}


void EditModeManager::addFunction() {
	if (mode_selected->addMode) {
		if (mode_selected == mode_objects) {
			int p1, p2;
			if (gridNo == 0) {
				p1 = 0;
				p2 = 1;
			}
			else if (gridNo == 1){
				p1 = 1;
				p2 = 2;
			}
			else if (gridNo == 2) {
				p1 = 2;
				p2 = 3;
			}
			entityManager->addEntityObject(edit_ModelIterator->first, mode_selected->edit_pos, mode_selected->edit_scale, glm::vec3(mode_selected->edit_pitch, mode_selected->edit_yaw, mode_selected->edit_roll), 1.0f, gridNo, p1, p2, 2, -1, -1, -1);
			edit_ObjectIterator = entityManager->getEnviObject_Begin();
		}
		if (mode_selected == mode_bounding) {
			if (mode_selected->visualize) {
				colManager->addBox(mode_selected->edit_scale.x, mode_selected->edit_scale.y, mode_selected->edit_scale.z, mode_selected->edit_pos.x, mode_selected->edit_pos.y, mode_selected->edit_pos.z, 0.0f);
				edit_RigidIterator = colManager->getRigidBegin();
			}
			else {
				colManager->addHeirarchicalGrid(glm::vec2(mode_selected->edit_pos.x, mode_selected->edit_pos.z), glm::vec2(mode_selected->edit_scale.x, mode_selected->edit_scale.z));
				edit_HeirIterator = colManager->getHeirBegin();
			}
		}
	}
}
void EditModeManager::undoFunction() {
	;
	/*if (mode_selected == mode_objects) {
		glm::mat4 edit_ModelMatrix;
		Common::createModelMatrix(edit_ModelMatrix, (*edit_ObjectIterator)->getInitialPos(), (*edit_ObjectIterator)->getInitialYaw(), (*edit_ObjectIterator)->getInitialScale());
		(*edit_ObjectIterator)->setModelMatrix(edit_ModelMatrix);
	}*/
}

void EditModeManager::recalcSides_Grid() {
	(*edit_HeirIterator)->left = (*edit_HeirIterator)->position.x - (*edit_HeirIterator)->scale.x;
	(*edit_HeirIterator)->right = (*edit_HeirIterator)->position.x + (*edit_HeirIterator)->scale.x;
	(*edit_HeirIterator)->top = (*edit_HeirIterator)->position.y - (*edit_HeirIterator)->scale.y;
	(*edit_HeirIterator)->bottom = (*edit_HeirIterator)->position.y + (*edit_HeirIterator)->scale.y;
}

void EditModeManager::moveForward() {
	if (mode_selected == mode_bounding) {
		if (mode_selected->visualize) {
			btTransform t;
			(*edit_RigidIterator)->getMotionState()->getWorldTransform(t);
			t.setOrigin(btVector3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z() - 0.05f));
			(*edit_RigidIterator)->getMotionState()->setWorldTransform(t);
		}
		else {
			(*edit_HeirIterator)->position.y -= scaleRate;
			recalcSides_Grid();
		}
	}
	else if (mode_selected == mode_objects) {
		glm::vec3 pos((*edit_ObjectIterator)->getPos());
		pos.z -= scaleRate;
		glm::mat4 modelMatrix;
		glm::vec3 rotation((*edit_ObjectIterator)->getPitch(), (*edit_ObjectIterator)->getYaw(), (*edit_ObjectIterator)->getRoll());
		Common::createModelMatrix(modelMatrix, pos, (*edit_ObjectIterator)->getScale(), rotation);
		(*edit_ObjectIterator)->setModelMatrix(modelMatrix);
	}
}
void EditModeManager::moveBackward() {
	if (mode_selected == mode_bounding) {
		if (mode_selected->visualize) {
			btTransform t;
			(*edit_RigidIterator)->getMotionState()->getWorldTransform(t);
			t.setOrigin(btVector3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z() + 0.05f));
			(*edit_RigidIterator)->getMotionState()->setWorldTransform(t);
		}
		else {
			(*edit_HeirIterator)->position.y += scaleRate;
			recalcSides_Grid();
		}
	}
	else if (mode_selected == mode_objects) {
		glm::vec3 pos((*edit_ObjectIterator)->getPos());
		pos.z += scaleRate;
		glm::mat4 modelMatrix;
		glm::vec3 rotation((*edit_ObjectIterator)->getPitch(), (*edit_ObjectIterator)->getYaw(), (*edit_ObjectIterator)->getRoll());
		Common::createModelMatrix(modelMatrix, pos, (*edit_ObjectIterator)->getScale(), rotation);
		(*edit_ObjectIterator)->setModelMatrix(modelMatrix);
	}
}
void EditModeManager::moveLeft() {
	if (mode_selected == mode_bounding) {
		if (mode_selected->visualize) {
			btTransform t;
			(*edit_RigidIterator)->getMotionState()->getWorldTransform(t);
			t.setOrigin(btVector3(t.getOrigin().x() - 0.05f, t.getOrigin().y(), t.getOrigin().z()));
			(*edit_RigidIterator)->getMotionState()->setWorldTransform(t);
		}
		else {
			(*edit_HeirIterator)->position.x -= scaleRate;
			recalcSides_Grid();
		}
	}
	else if (mode_selected == mode_objects) {
		std::cout<<"asdddddddddddd";
		glm::vec3 pos((*edit_ObjectIterator)->getPos());
		pos.x -= scaleRate;
		glm::mat4 modelMatrix;
		glm::vec3 rotation((*edit_ObjectIterator)->getPitch(), (*edit_ObjectIterator)->getYaw(), (*edit_ObjectIterator)->getRoll());
		Common::createModelMatrix(modelMatrix, pos, (*edit_ObjectIterator)->getScale(), rotation);
		(*edit_ObjectIterator)->setModelMatrix(modelMatrix);
	}
}
void EditModeManager::moveRight() {
	if (mode_selected == mode_bounding) {
		if (mode_selected->visualize) {
			btTransform t;
			(*edit_RigidIterator)->getMotionState()->getWorldTransform(t);
			t.setOrigin(btVector3(t.getOrigin().x() + 0.05f, t.getOrigin().y(), t.getOrigin().z()));
			(*edit_RigidIterator)->getMotionState()->setWorldTransform(t);
		}
		else {
			(*edit_HeirIterator)->position.x += scaleRate;
			recalcSides_Grid();
		}
	}
	else if (mode_selected == mode_objects) {
		glm::vec3 pos((*edit_ObjectIterator)->getPos());
		pos.x += scaleRate;
		glm::mat4 modelMatrix;
		glm::vec3 rotation((*edit_ObjectIterator)->getPitch(), (*edit_ObjectIterator)->getYaw(), (*edit_ObjectIterator)->getRoll());
		Common::createModelMatrix(modelMatrix, pos, (*edit_ObjectIterator)->getScale(), rotation);
		(*edit_ObjectIterator)->setModelMatrix(modelMatrix);
	}
}

void EditModeManager::setIndexOfSelectedObject(unsigned int index) { indexOfSelectedObject = index; }
