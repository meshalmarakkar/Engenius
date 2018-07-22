#include "EntityManager.h"

void EntityManager::addEntityObject(const std::string& mdlName, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, const float& culBound, const unsigned int& gridNo, const int& p1, const int& p2, const int& p3, const int& s1, const int& s2, const int& s3, float tiling) {
	std::pair<int, int> lights;
	std::pair<int, int> lights2;

	objects.emplace_back(new Entity(modelManager->getModel_envi(mdlName), pos, scale, rot, gridNo));
	objects.back()->setCullingBound(culBound);
	//lights = lightingManager->getClosestLights(objects.back()->getPos());
	objects.back()->setPointLightIDs(p1, p2, p3);
	//lights2 = lightingManager->getClosestSpotLights(objects.back()->getPos());
	objects.back()->setSpotLightIDs(s1, s2, s3);
	objects.back()->setTiling(tiling);

	objects.resize(objects.size());
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
			glm::ivec3 pointLightIDs;

			modelName = objects.at(objNo)->getModel()->getName();
			position = objects.at(objNo)->getPos();
			scale = objects.at(objNo)->getScale();
			rotation = glm::vec3(objects.at(objNo)->getPitch(), objects.at(objNo)->getYaw(), objects.at(objNo)->getRoll());
			pointLightIDs = *(objects.at(objNo)->getPointLightIDs());

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
			myfile << pointLightIDs.x << ",";
			myfile << pointLightIDs.y << ",";
			myfile << pointLightIDs.z << ",";
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
				float tiling = 1.0f;
				if (modelName == "surround") {
					cullingBound = cullingBound + (cullingBound * 0.5f);
					tiling = (cullingBound / 2.0f) * 2.0f;
				}
				addEntityObject(modelName, position, scale, rotation, cullingBound, gridNo, pointL1, pointL2, pointL3, -1, -1, -1, tiling);

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

void EntityManager::initPlayer(CollisionManager* colManager) {
	glm::vec3 pos(-2.0f, 0.0f, 2.0f);
	player = new Character(modelManager->getModel_char("batman"), pos, glm::vec3(1.0f), 90.0f);
	colManager->attachPlayerCollider(player, pos, 0.75f, 0.5f, 80.0f);

	player->setPointLightIDs(0,1,2);
	player->setSpotLightIDs(-1,-1,-1);
//	std::tuple<int, int, int> lights(lightingManager->getClosestLights(player->getPos()));
//	player->setPointLightIDs(std::get<0>(lights), std::get<1>(lights), std::get<2>(lights));
//	lights = lightingManager->getClosestSpotLights(player->getPos());
//	player->setSpotLightIDs(std::get<0>(lights), std::get<1>(lights), std::get<2>(lights));
}

EntityManager::EntityManager(Camera *camera, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager* colManager, TerrainManager* terrainManager, Renderer* renderer) : lightingManager(lightingManager), shaderManager(shaderManager), terrainManager(terrainManager), renderer(renderer){

	frustumCulling = new FrustumCulling();
	frustumCulling->setCamInternals(camera->getFOV() / DEG_TO_RADIAN, camera->getRatio(), camera->getNearPlane(), camera->getFarPlane()); //divide to send as degree
	
	modelManager = new ModelManager();

	colManager->addCollisionModel(modelManager->getModel_envi("cube"));
	
	readFile();
	//writeFile();
	
//CHECK FOR THE OBJECTS AFFECTED BY LIGHT HERE!!!!

	initPlayer(colManager);
}

bool shad = true;
void EntityManager::shadowTest() {
	shad = !shad;
}

void EntityManager::renderObjects(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	unsigned int shader;
	Shader* program;

	if (ifDeferred) {
		program = shaderManager->getShaderProgram(Programs::deferred_gBuffer_mapped);
		program->bind();
		program->uniformsToShader_RUNTIME();
		shader = program->getShaderProgram();
	}
	else {
		program = shaderManager->getShaderProgram(Programs::model_mapped);
		program->bind();
		program->uniformsToShader_RUNTIME();
		shader = program->getShaderProgram();
	}

	//program->uniform("far_plane", 25.0f);
	std::unordered_map<Model*, bool> renderedModels;

	for (unsigned int iter = 0; iter < objects.size(); iter++) {
		if (frustumCulling->sphereInFrustum(objects.at(iter)->getPos(), objects.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (objects.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = objects.at(iter)->getModel();
					renderedModels.insert(make_pair(objModel, true));

					objModel->batch_modelMatrices(objects.at(iter)->getModelMatrix());
					objModel->batch_pointIDs(*(objects[iter]->getPointLightIDs()));
					objModel->batch_spotIDs(*(objects[iter]->getSpotLightIDs()));
					objModel->batch_tilings(objects.at(iter)->getTiling());

					rendered = true;
				}
			}
		}
	}

	//TODO: FIX THIS!!!! fix the instanced rendering as pointlightID doesnt work.
	//It is set to 0, 1, 2 for now so the corresponding shadow maps used here.
	lightIDsToShader(shader, glm::ivec3(0, 1, 2), glm::ivec3(-1, -1, -1));

//	program->uniform("displayShadow", shad);

	for (std::unordered_map<Model*, bool>::iterator it = renderedModels.begin(); it != renderedModels.end(); it++) {
		Model* model = (*it).first;
		auto modelMatrices = model->getModelMatrices();
		auto pointIDs = model->getPointIDs();
		auto spotIDs = model->getSpotIDs();
		auto tilings = model->getTilings();
		auto meshes = model->getMeshes();
		const unsigned int instances = modelMatrices->size();

		if (instances > 5) {
			program->uniform("instanced", true);
			for (unsigned int i = 0; i < meshes->size(); i++) {
				Mesh* m = &(meshes->at(i));
				m->prepareInstancedDraw(modelMatrices, pointIDs, spotIDs, tilings);

				renderer->drawElements_instanced(program, m->getVAO(), m->getMaterial(), instances);
			}
		}
		else {
			program->uniform("instanced", false);
			for (unsigned int i = 0; i < instances; i++) {
				program->uniform("uniform_model", modelMatrices->at(i));
				program->uniform("tiling", tilings->at(i));
				program->uniform("pointLightIDs[0]", pointIDs->at(i).x);
				program->uniform("pointLightIDs[1]", pointIDs->at(i).y);
				program->uniform("pointLightIDs[2]", pointIDs->at(i).z);
				program->uniform("spotLightIDs[0]", int(spotIDs->at(i).x));
				program->uniform("spotLightIDs[1]", int(spotIDs->at(i).y));
				program->uniform("spotLightIDs[2]", int(spotIDs->at(i).z));

				for (unsigned int id = 0; id < meshes->size(); id++) {
					Mesh* m = &(meshes->at(id));

					//program->uniformsToShader(rp->getUniforms());
					renderer->drawElements(program, m->getVAO(), m->getMaterial());
				}
			}
		}

		model->cleanup();
	}
}

void EntityManager::renderCharacters(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	//Player
	Shader* program;

	unsigned int shader;
	if (ifDeferred) {
		//TODO: NEED TO CREATE GBUFFER SHADER FOR ANIMATED MODELS 
	}
	else {
		program = shaderManager->getShaderProgram(Programs::model_animated);
		program->bind();
		program->uniformsToShader_RUNTIME();
		shader = program->getShaderProgram();
	}

	if (frustumCulling->sphereInFrustum(glm::vec3(-1.0f, 0.5f, 8.5f), player->getCullingBound()) != 0) {
		lightIDsToShader(shader, *(player->getPointLightIDs()), *(player->getSpotLightIDs()));
		player->boneLocationToShader(shader);
		program->uniform("instanced", false);
		program->uniform("uniform_model", (*(player->getRenderProperties())->getModelMatrix()));
		program->uniformsToShader(player->getRenderProperties()->getUniforms());
		
		Model* pm = player->getModel();
		for (unsigned int i = 0; i < pm->getMeshes()->size(); i++) {
			Mesh* m = &(pm->getMeshes()->at(i));
			renderer->drawElements(program, m->getVAO(), m->getMaterial());
		}
	}
}

void EntityManager::lightIDsToShader(const unsigned int& shader, const glm::ivec3& pointLightIDs, const glm::ivec3& spotLightIDs) {
	const glm::ivec3 pl = pointLightIDs;
	const glm::ivec3 sl = spotLightIDs;
	if (lightingManager->getIfShadow() == true) {
		lightingManager->shadowMapsToShader(shader, pl.x, pl.y, pl.z, sl.x, sl.y, sl.z);
	}
	else {
		lightingManager->noShadowMessage(shader);
	}
}

void EntityManager::shadow_draw(Shader* program, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[]) {
	//TERRAINS
//	terrainManager->shadowDraw(shader, renderer);

	//OBJECTS
	renderer->disableCullFace();

	std::unordered_map<Model*, bool> renderedModels;

	for (unsigned int iter = 0; iter < objects.size(); iter++) {
	//	if (frustumCulling->sphereInFrustum(objects.at(iter)->getPos(), objects.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (objects.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = objects.at(iter)->getModel();
					if (objModel->getName() != "surround" && objModel->getName() != "ceilingLight") {
						renderedModels.insert(make_pair(objModel, true));

						objModel->batch_modelMatrices(objects.at(iter)->getModelMatrix());
					}
					rendered = true;
				}
			}
	//	}
	}

	program->uniform("animated", false);

	for (std::unordered_map<Model*, bool>::iterator it = renderedModels.begin(); it != renderedModels.end(); it++) {
		Model* model = (*it).first;
		auto modelMatrices = model->getModelMatrices();
		auto meshes = model->getMeshes();
		const unsigned int instances = modelMatrices->size();

		if (instances > 5) {
			program->uniform("instanced", true);
			for (unsigned int i = 0; i < meshes->size(); i++) {
				Mesh* m = &(meshes->at(i));
				m->prepareInstancedDraw(modelMatrices);
				renderer->drawElements_instanced(program, m->getVAO(), m->getMaterial(), instances);
			}
		}
		else {
			program->uniform("instanced", false);
			for (unsigned int i = 0; i < instances; i++) {
				for (unsigned int id = 0; id < meshes->size(); id++) {
					Mesh* m = &(meshes->at(id));
					program->uniform("uniform_model", modelMatrices->at(i));
					renderer->drawElements(program, m->getVAO(), m->getMaterial());
				}
			}
		}

		model->cleanup();
	}

	renderer->enableCullFace();
	//PLAYER
//	player->draw(shader, renderer);
}

void EntityManager::draw(const float& dt_secs, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred) {
	if (!ifDeferred)
		terrainManager->render(renderer, player->getPos(), dt_secs);
	renderObjects(NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);
	if (!ifDeferred)
		renderCharacters(NUM_EFFECTIVE_GRIDS, renderGridNo, ifDeferred);
}

void EntityManager::Update(Camera* camera, const float& dt_secs) {
	frustumCulling->setCamDef(camera->getCameraEye(), camera->getCameraDirection(), camera->getCameraUp());

	float terrHeightAtPlayerPos = terrainManager->getTerrainHeight(player->getPos().x, player->getPos().z);

	player->Update(camera->getYaw(), camera->getCameraAt(), camera->getCameraUp(), terrHeightAtPlayerPos, dt_secs);
}

void EntityManager::changeAnimation() {
	player->changeAnimation();
}

Entity* EntityManager::getObject(const unsigned int& i) {	return objects.at(i); }

Character* EntityManager::getPlayer() { return this->player; }
Model* EntityManager::getENVModel(const std::string& name) { return modelManager->getModel_envi(name); }

void EntityManager::setEntityPos(const glm::vec3& pos, const int& indexOfSelectedObject) {
	objects.at(indexOfSelectedObject)->setPos(pos);
	glm::mat4 model;
	Common::createModelMatrix(model, pos, objects.at(indexOfSelectedObject)->getYaw(), objects.at(indexOfSelectedObject)->getScale());
	objects.at(indexOfSelectedObject)->setModelMatrix(model);
}

std::unordered_map<std::string, Model*>::iterator EntityManager::getEnviModel_Begin() {	return modelManager->getEnviModel_Begin(); }
std::unordered_map<std::string, Model*>::iterator EntityManager::getEnviModel_End() { return modelManager->getEnviModel_End(); }
std::vector<Entity*>::iterator EntityManager::getEnviObject_Begin() { return objects.begin(); }
std::vector<Entity*>::iterator EntityManager::getEnviObject_End() { return objects.end(); }