#include "EntityManager.h"

void EntityManager::addEntityObject(const std::string& mdlName, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, const float& culBound, const unsigned int& gridNo, const int& p1, const int& p2, const int& p3, const int& s1, const int& s2, const int& s3) {
	std::pair<int, int> lights;
	std::pair<int, int> lights2;

	objects.emplace_back(new Entity(modelManager->getModel_envi(mdlName), pos, scale, rot, gridNo));
	objects.back()->setCullingBound(culBound);
	//lights = lightingManager->getClosestLights(objects.back()->getPos());
	objects.back()->setPointLightIDs(p1, p2, p3);
	//lights2 = lightingManager->getClosestSpotLights(objects.back()->getPos());
	objects.back()->setSpotLightIDs(s1, s2, s3);

	objects.resize(objects.size());
}

void EntityManager::addEntityPanels(const std::string& mdlName, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, const float& culBound, const unsigned int& gridNo, const int& p1, const int& p2, const int& p3, const int& s1, const int& s2, const int& s3) {
	std::pair<int, int> lights;
	std::pair<int, int> lights2;

	float tiling = (culBound / 2.0f) * 2.0f;

	panels.emplace_back(new Entity(modelManager->getModel_envi(mdlName), pos, scale, rot, gridNo));
	panels.back()->setCullingBound(culBound + (culBound * 0.5f));
	//	lights = lightingManager->getClosestLights(walls.back()->getPos());
	panels.back()->setPointLightIDs(p1, p2, p3);
	//	lights2 = lightingManager->getClosestSpotLights(walls.back()->getPos());
	panels.back()->setSpotLightIDs(s1, s2, s3);
	panels.back()->setTiling(tiling);
}

bool EntityManager::writeFile() {
	// writing on a text file
	ofstream myfile("../Engenius/Levels/Level.dat");
	if (myfile.is_open())
	{
		myfile << "ModelName | Positions | Scale | Rotation | CullBound | GridNum | PointL1 | PointL2 | PointL3 \t//numberOfBodies on first line\n";
		myfile << objects.size() << "#\n";

		for (unsigned int objNo = 0; objNo < objects.size(); objNo++) {
			std::string modelName;
			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 rotation;

			modelName = objects.at(objNo)->getModel()->getName();
			position = objects.at(objNo)->getPos();
			scale = objects.at(objNo)->getScale();
			rotation = glm::vec3(objects.at(objNo)->getPitch(), objects.at(objNo)->getYaw(), objects.at(objNo)->getRoll());

			myfile << modelName << ",";
			myfile << position.x << ",";
			myfile << position.y << ",";
			myfile << position.z << ",";
			myfile << scale.x << ",";
			myfile << scale.y << ",";
			myfile << scale.z << ",";
			myfile << rotation.x << ",";
			myfile << rotation.y << ",";
			myfile << rotation.z << ",";
			myfile << objects.at(objNo)->getCullingBound() << ",";
			myfile << objects.at(objNo)->getGridNo() << ",";
			myfile << objects.at(objNo)->getPointLightID(0) << ",";
			myfile << objects.at(objNo)->getPointLightID(1) << ",";
			myfile << objects.at(objNo)->getPointLightID(2) << ",";
			myfile << "\n";
			cout << "Boxes Saved\n";
		}
		myfile.close();
		return true;
	}
	else {
		cout << "Unable to open file";
		return false;
	}
}

bool EntityManager::readFile() {
	// reading a text file
	string line;
	ifstream myfile;
	myfile = ifstream("../Engenius/Levels/Level.dat");

	if (myfile.is_open())
	{
		int numberOfBodies;
		bool gotNumber = false;
		int bodyNo = 0;
		while (getline(myfile, line, '\n')) //delimiter is '\n' in this
		{
			std::string modelName;
			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 rotation;
			float cullingBound;
			unsigned int gridNo;
			int pointL1, pointL2, pointL3;

			string asString;
			int variables = 15;
			if (!gotNumber) {
				string numAsString;
				getline(myfile, numAsString, '#');
				numberOfBodies = stoi(numAsString);
				gotNumber = true;
			}
			else if (bodyNo != numberOfBodies) {
				for (int i = 0; i < variables; i++) {
					getline(myfile, asString, ',');
					switch (i) {
					case 0: modelName = asString;			break;
					case 1: position.x = stof(asString);	break;
					case 2: position.y = stof(asString);	break;
					case 3: position.z = stof(asString);	break;
					case 4: scale.x = stof(asString);		break;
					case 5: scale.y = stof(asString);		break;
					case 6: scale.z = stof(asString);		break;
					case 7: rotation.x = stof(asString);	break;
					case 8: rotation.y = stof(asString);	break;
					case 9: rotation.z = stof(asString);	break;
					case 10: cullingBound = stof(asString);	break;
					case 11: gridNo = stoi(asString);		break;
					case 12: pointL1 = stoi(asString);		break;
					case 13: pointL2 = stoi(asString);		break;
					case 14: pointL3 = stoi(asString);		break;
					}
				}//for loop
				bodyNo++;
				addEntityObject(modelName, position, scale, rotation, cullingBound, gridNo, pointL1, pointL2, pointL3, -1, -1, -1);
				//std::cout << modelName << "," << position.x << "," << position.y << "," << position.z << "," << scale.x << "," << scale.y << "," << scale.z << "," << rotation.x << "," << rotation.y << "," << rotation.z << ", " << cullingBound << ", " << gridNo << std::endl;
			}
		}//while loop
		return true;
	}
	else {
		cout << "\nMission failed. We'll get em next time. \n. Unable to open file";
		return false;
	}
}

bool EntityManager::writeFile_Panels() {
	// writing on a text file
	ofstream myfile("../Engenius/Levels/LevelPanels.dat");
	if (myfile.is_open())
	{
		myfile << "ModelName | Positions | Scale | Rotation | CullBound | GridNum | PointL1 | PointL2 | PointL3 \t//numberOfBodies on first line\n";
		myfile << panels.size() << "#\n";

		for (unsigned int objNo = 0; objNo < panels.size(); objNo++) {
			std::string modelName;
			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 rotation;

			modelName = panels.at(objNo)->getModel()->getName();
			position = panels.at(objNo)->getPos();
			scale = panels.at(objNo)->getScale();
			rotation = glm::vec3(panels.at(objNo)->getPitch(), panels.at(objNo)->getYaw(), panels.at(objNo)->getRoll());

			myfile << modelName << ",";
			myfile << position.x << ",";
			myfile << position.y << ",";
			myfile << position.z << ",";
			myfile << scale.x << ",";
			myfile << scale.y << ",";
			myfile << scale.z << ",";
			myfile << rotation.x << ",";
			myfile << rotation.y << ",";
			myfile << rotation.z << ",";
			myfile << panels.at(objNo)->getCullingBound() << ",";
			myfile << panels.at(objNo)->getGridNo() << ",";
			myfile << panels.at(objNo)->getPointLightID(0) << ",";
			myfile << panels.at(objNo)->getPointLightID(1) << ",";
			myfile << panels.at(objNo)->getPointLightID(2) << ",";
			myfile << "\n";
			cout << "Boxes Saved\n";
		}
		myfile.close();
		return true;
	}
	else {
		cout << "Unable to open file";
		return false;
	}
}

bool EntityManager::readFile_Panels() {
	// reading a text file
	string line;
	ifstream myfile;
	myfile = ifstream("../Engenius/Levels/LevelPanels.dat");

	if (myfile.is_open())
	{
		int numberOfBodies;
		bool gotNumber = false;
		int bodyNo = 0;
		while (getline(myfile, line, '\n')) //delimiter is '\n' in this
		{
			std::string modelName;
			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 rotation;
			float cullingBound;
			unsigned int gridNo;
			int pointL1, pointL2, pointL3;

			string asString;
			int variables = 15;
			if (!gotNumber) {
				string numAsString;
				getline(myfile, numAsString, '#');
				numberOfBodies = stoi(numAsString);
				gotNumber = true;
			}
			else if (bodyNo != numberOfBodies) {
				for (int i = 0; i < variables; i++) {
					getline(myfile, asString, ',');
					switch (i) {
					case 0: modelName = asString;			break;
					case 1: position.x = stof(asString);	break;
					case 2: position.y = stof(asString);	break;
					case 3: position.z = stof(asString);	break;
					case 4: scale.x = stof(asString);		break;
					case 5: scale.y = stof(asString);		break;
					case 6: scale.z = stof(asString);		break;
					case 7: rotation.x = stof(asString);	break;
					case 8: rotation.y = stof(asString);	break;
					case 9: rotation.z = stof(asString);	break;
					case 10: cullingBound = stof(asString);	break;
					case 11: gridNo = stoi(asString);		break;
					case 12: pointL1 = stoi(asString);		break;
					case 13: pointL2 = stoi(asString);		break;
					case 14: pointL3 = stoi(asString);		break;
					}
				}//for loop
				bodyNo++;
				addEntityPanels(modelName, position, scale, rotation, cullingBound, gridNo, pointL1, pointL2, pointL3, -1, -1, -1);

				//std::cout << modelName << "," << position.x << "," << position.y << "," << position.z << "," << scale.x << "," << scale.y << "," << scale.z << "," << rotation.x << "," << rotation.y << "," << rotation.z << ", " << cullingBound << ", " << gridNo << std::endl;
			}
		}//while loop
		return true;
	}
	else {
		cout << "\nMission failed. We'll get em next time. \n. Unable to open file";
		return false;
	}
}

void EntityManager::initObjectsToWorld() {
	readFile();
	readFile_Panels();
	//writeFile_Panels();

	panels.resize(panels.size());
	objects.resize(objects.size());
	//objects_disableCullface.resize(panels.size());
}

void EntityManager::initPlayer(CollisionManager* colManager) {
	glm::vec3 pos(-2.0f, 0.0f, 2.0f);
	player = new Character(modelManager->getModel_char("batman"), pos, glm::vec3(1.0f), 90.0f);
	colManager->attachPlayerCollider(player, pos, 0.75f, 0.5f, 80.0f);

	std::tuple<int, int, int> lights(lightingManager->getClosestLights(player->getPos()));
	player->setPointLightIDs(std::get<0>(lights), std::get<1>(lights), std::get<2>(lights));
	lights = lightingManager->getClosestSpotLights(player->getPos());
	player->setSpotLightIDs(std::get<0>(lights), std::get<1>(lights), std::get<2>(lights));
}

EntityManager::EntityManager(Camera *camera, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager* colManager, TerrainManager* terrainManager) : camera(camera), lightingManager(lightingManager), shaderManager(shaderManager), terrainManager(terrainManager){

	frustumCulling = new FrustumCulling();
	frustumCulling->setCamInternals(camera->getFOV() / DEG_TO_RADIAN, camera->getRatio(), camera->getNearPlane(), camera->getFarPlane()); //divide to send as degree
	
	modelManager = new ModelManager();

	colManager->addCollisionModel(modelManager->getModel_envi("cube"));
	
	initObjectsToWorld();
//CHECK FOR THE OBJECTS AFFECTED BY LIGHT HERE!!!!

	initPlayer(colManager);
}

void EntityManager::renderPanels(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	unsigned int shader;
	Shader* program;

	if (ifDeferred) {
		shader = shaderManager->get_gBuffer_mapped_program();
		//unsigned int shader = shaderManager->get_gBuffer_program();
		shaderManager->gl_UseProgram(shader);
		camera->passViewProjToShader(shader);
		farPlane_camEye_toShader(shader);
	}
	else {
		program = shaderManager->getShaderProgram(Programs::model_mapped);
		program->bind();
		program->uniformsToShader_RUNTIME();
		shader = program->getShaderProgram();
	}

	modelManager->getModel_envi("surround")->bindWall(shader);
	for (unsigned int iter = 0; iter < panels.size(); iter++) {
		if (frustumCulling->sphereInFrustum(panels.at(iter)->getPos(), panels.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (panels.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = panels.at(iter)->getModel();
					const glm::mat4 objMatrix = panels.at(iter)->getModelMatrix();
					lightIDsToShader(shader, panels[iter]->getPointLightID(0), panels[iter]->getPointLightID(1), panels[iter]->getPointLightID(2), panels[iter]->getSpotLightID(0), panels[iter]->getSpotLightID(1), panels[iter]->getSpotLightID(2));
					glUniform1f(glGetUniformLocation(shader, "tiling"), panels.at(iter)->getTiling());
					objModel->drawWall(shader, objMatrix, renderer);
					rendered = true;
				}
			}
		}
	}
	modelManager->getModel_envi("surround")->unbindWall();
}

void EntityManager::renderObjects(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	unsigned int shader;
	Shader* program;

	if (ifDeferred) {
		shader = shaderManager->get_gBuffer_mapped_program();
		//unsigned int shader = shaderManager->get_gBuffer_program();
		shaderManager->gl_UseProgram(shader);
		camera->passViewProjToShader(shader);
		farPlane_camEye_toShader(shader);
	}
	else {
		program = shaderManager->getShaderProgram(Programs::model_mapped);
		program->bind();
		program->uniformsToShader_RUNTIME();
		shader = program->getShaderProgram();
	}

	for (unsigned int iter = 0; iter < objects.size(); iter++) {
		if (frustumCulling->sphereInFrustum(objects.at(iter)->getPos(), objects.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (objects.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = objects.at(iter)->getModel();
					const glm::mat4 objMatrix = objects.at(iter)->getModelMatrix();
					lightIDsToShader(shader, objects[iter]->getPointLightID(0), objects[iter]->getPointLightID(1), objects[iter]->getPointLightID(2), objects[iter]->getSpotLightID(0), objects[iter]->getSpotLightID(1), objects[iter]->getSpotLightID(2));
					glUniform1f(glGetUniformLocation(shader, "tiling"), objects.at(iter)->getTiling());
					objModel->Draw(shader, objMatrix, renderer);
					rendered = true;
				}
			}
		}
	}
}

void EntityManager::renderCharacters(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	//Player
	Shader* program;

	unsigned int shader;
	if (ifDeferred) {
		unsigned int shader = shaderManager->get_animated_model_program();
		shaderManager->gl_UseProgram(shader);

		farPlane_camEye_toShader(shader);
		camera->passViewProjToShader(shader);
	}
	else {
		program = shaderManager->getShaderProgram(Programs::model_animated);
		program->bind();
		program->uniformsToShader_RUNTIME();
		shader = program->getShaderProgram();
	}

	if (frustumCulling->sphereInFrustum(glm::vec3(-1.0f, 0.5f, 8.5f), player->getCullingBound()) != 0) {
		lightIDsToShader(shader, player->getPointLightID(0), player->getPointLightID(1), player->getPointLightID(2), player->getSpotLightID(0), player->getSpotLightID(1), player->getSpotLightID(2));
		player->draw(shader, renderer);
	}
}

void EntityManager::lightIDsToShader(const unsigned int& shader, const int& point_id1, const int& point_id2, const int& point_id3, const int& spot_id1, const int& spot_id2, const int& spot_id3) {
	lightingManager->lightIDsToShader(shader, point_id1, point_id2, point_id3, spot_id1, spot_id2, spot_id3);
	if (lightingManager->getIfShadow() == true) {
		lightingManager->shadowMapsToShader(shader, point_id1, point_id2, point_id3, spot_id1, spot_id2, spot_id3);
	}
	else {
		lightingManager->noShadowMessage(shader);
	}
}

void EntityManager::farPlane_camEye_toShader(const unsigned int& shader) {
	glUniform1f(glGetUniformLocation(shader, "far_plane"), camera->getFarPlane());
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera->getCameraEye()));
}

void EntityManager::shadow_draw(const unsigned int& shader, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[]) {
	farPlane_camEye_toShader(shader);

	//TERRAINS
//	terrainManager->shadowDraw(shader, renderer);

	//OBJECTS
	glDisable(GL_CULL_FACE);

	for (unsigned int iter = 0; iter < objects.size(); iter++) {
		if (frustumCulling->sphereInFrustum(objects.at(iter)->getPos(), objects.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (objects.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = objects.at(iter)->getModel();
					const glm::mat4 objMatrix = objects.at(iter)->getModelMatrix();
				
				//	lightIDsToShader(shader, objects[iter]->getPointLightID(0), objects[iter]->getPointLightID(1), objects[iter]->getSpotLightID(0), objects[iter]->getSpotLightID(1));
				//	glUniform1f(glGetUniformLocation(shader, "tiling"), objects.at(iter)->getTiling());
					objModel->Draw(shader, objMatrix, renderer);
					rendered = true;
				}
			}
		}
	}

	glEnable(GL_CULL_FACE);
	//PLAYER
	player->draw(shader, renderer);
}

void EntityManager::draw(const float& dt_secs, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	if (!ifDeferred)
		terrainManager->render(renderer, player->getPos(), dt_secs);
	renderObjects(NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);
	renderPanels(NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);
	if (!ifDeferred)
		renderCharacters(NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);
}

void EntityManager::Update(const float& dt_secs) {
	frustumCulling->setCamDef(camera->getCameraEye(), camera->getCameraDirection(), camera->getCameraUp());

	float terrHeightAtPlayerPos = terrainManager->getTerrainHeight(player->getPos().x, player->getPos().z);

	player->Update(camera->getYaw(), camera->getCameraAt(), camera->getCameraUp(), terrHeightAtPlayerPos, dt_secs);
}

void EntityManager::changeAnimation() {
	player->changeAnimation();
}

Entity* EntityManager::getObject(const unsigned int& i) {	return panels.at(i); }

Character* EntityManager::getPlayer() { return this->player; }
Model* EntityManager::getENVModel(const std::string& name) { return modelManager->getModel_envi(name); }

void EntityManager::setEntityPos(const glm::vec3& pos, const int& indexOfSelectedObject) {
	panels.at(indexOfSelectedObject)->setPos(pos);
	glm::mat4 model;
	Common::createModelMatrix(model, pos, panels.at(indexOfSelectedObject)->getYaw(), panels.at(indexOfSelectedObject)->getScale());
	panels.at(indexOfSelectedObject)->setModelMatrix(model);
}

std::unordered_map<std::string, Model*>::iterator EntityManager::getEnviModel_Begin() {	return modelManager->getEnviModel_Begin(); }
std::unordered_map<std::string, Model*>::iterator EntityManager::getEnviModel_End() { return modelManager->getEnviModel_End(); }
std::vector<Entity*>::iterator EntityManager::getEnviObject_Begin() { return panels.begin(); }
std::vector<Entity*>::iterator EntityManager::getEnviObject_End() { return panels.end(); }