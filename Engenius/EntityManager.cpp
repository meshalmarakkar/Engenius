#include "EntityManager.h"

void initEnvironmentModels(unordered_map<std::string, Model*> &models) {
	const string envDirectory = "../Engenius/Models/Environment/";
	string path = "";

	//cout << "wall -> ";
	path = envDirectory + "Terrain/plane.obj";
	models.insert(std::pair<string, Model*>("surround", new Model("surround", path, "whiteTile/diffuse.png", "whiteTile/specular.png", "whiteTile/normal.png")));

	//cout << "cube -> ";
	path = envDirectory + "Terrain/cube.obj";
	models.insert(std::pair<string, Model*>("cube", new Model("cube", path, "whiteTile/diffuse.png", "whiteTile/specular.png", "whiteTile/normal.png")));

	//cout << "ceilingLight -> ";
	path = envDirectory + "light/ceilingLight.obj";
	models.insert(std::pair<string, Model*>("ceilingLight", new Model("ceilingLight", path, "diffuse.png", NULL, "normal.png")));

//	cout << "computer -> ";
	path = envDirectory + "Computer_Laptop/Computer_Laptop.obj";
	models.insert(std::pair<string, Model*>("computer", new Model("computer", path)));

//	cout << "securityDoor -> ";
	path = envDirectory + "Door/big_security_door.obj";
	models.insert(std::pair<string, Model*>("securityDoor", new Model("securityDoor", path, "door_cell_d.png", NULL, "door_cell_n.png")));

//	cout << "controlPanel -> ";
	path = envDirectory + "Control Panel/Machine 08 Straight.obj";
	models.insert(std::pair<string, Model*>("controlPanel", new Model("controlPanel", path, "ABY_Machine_08_A_Diffuse.png", NULL, "ABY_Machine_08_A_NormalsMap.png")));

//	cout << "electronicDevice -> ";
	path = envDirectory + "ElectronicDevice/hacking tool.obj";
	models.insert(std::pair<string, Model*>("electronicDevice", new Model("electronicDevice", path)));

//	cout << "closedLocker -> ";
	path = envDirectory + "Cabinet_non-open/armory_cabinet.obj";
	models.insert(std::pair<string, Model*>("closedLocker", new Model("closedLocker", path, "armory_cabinet_d.png", "armory_cabinet_m2.png", "armory_cabinet_n.png")));

//	cout << "wallTech -> ";
	path = envDirectory + "WallTech/Wall_Tech_M.obj";
	models.insert(std::pair<string, Model*>("wallTech", new Model("wallTech", path, "WallTech_Diffuse.png", NULL, "WallTech_NormalMap.png")));

//	cout << "speaker -> ";
	path = envDirectory + "Speaker/Speaker.obj";
	models.insert(std::pair<string, Model*>("speaker", new Model("speaker", path)));
	
//	cout << "server -> ";
	path = envDirectory + "Server/ServerV2+console.obj";
	models.insert(std::pair<string, Model*>("server", new Model("server", path)));

//	cout << "desk -> ";
	path = envDirectory + "BasicDesk/Desk.obj";
	models.insert(std::pair<string, Model*>("desk", new Model("desk", path, "diffuse.jpg", "specular.jpg", "normal.jpg")));

}

void initCharacterModels(unordered_map<std::string, AnimatedModel*> &models) {
	const string charDirectory = "../Engenius/Models/Environment/";
	string path = "";

	cout << "dummycubechar -> ";
	path = charDirectory + "Terrain/cube.obj";

	models.insert(std::pair<string, AnimatedModel*>("cube", new AnimatedModel("cube", path)));
	//models.insert(std::pair<string, AnimatedModel*>("batman", new AnimatedModel("character", path)));
	//const string charDirectory = "../Engenius/Models/Characters/"; 
	//string path = "";

	//cout << "steamPunkBatman -> ";
	//path = charDirectory + "Batman/warrior_idle.dae";
	//models.insert(std::pair<string, AnimatedModel*>("batman", new AnimatedModel("character", path)));
	//path = charDirectory + "Batman/walking.dae";
	//models.insert(std::pair<string, AnimatedModel*>("batmanWalk", new AnimatedModel(path, true)));
	//path = charDirectory + "Batman/running.dae";
	//models.insert(std::pair<string, AnimatedModel*>("batmanJog", new AnimatedModel(path, true)));
	//path = charDirectory + "Batman/standard_run.dae";
	//models.insert(std::pair<string, AnimatedModel*>("batmanRun", new AnimatedModel(path, true)));
	//path = charDirectory + "Batman/walking_backwards.dae";
	//models.insert(std::pair<string, AnimatedModel*>("batmanWalkBack", new AnimatedModel(path, true)));
	//models.at("batman")->setMovementAnims(models.at("batmanWalk")->getAnims());
	//models.at("batman")->setMovementAnims(models.at("batmanJog")->getAnims());
	//models.at("batman")->setMovementAnims(models.at("batmanRun")->getAnims());
	//models.at("batman")->setMovementAnims(models.at("batmanWalkBack")->getAnims());
	//models.erase("batmanWalk");
	//models.erase("batmanJog");
	//models.erase("batmanRun");
	//models.erase("batmanWalkBack");	
}

void EntityManager::addEntityObject(std::string modelName, glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, float cullingBound, unsigned int gridNo, int p1, int p2, int s1, int s2) {
	std::pair<int, int> lights;
	std::pair<int, int> lights2;

	entities.emplace_back(new Entity(enviModels.at(modelName), pos, scale, rotation, gridNo));
	entities.back()->setCullingBound(cullingBound);
	//lights = lightingManager->getClosestLights(objects.back()->getPos());
	entities.back()->setPointLightIDs(p1, p2);
	//lights2 = lightingManager->getClosestSpotLights(objects.back()->getPos());
	entities.back()->setSpotLightIDs(s1, s2);

	entities.resize(entities.size());
}

void EntityManager::addEntityWall(std::string modelName, glm::vec3 pos, glm::vec3 scale, std::string facing, float cullingBound, unsigned int gridNo, int p1, int p2, int s1, int s2) {
	std::pair<int, int> lights;
	std::pair<int, int> lights2;
	bool issurround = false;
	glm::vec3 rotation;
	if (facing == "negativeZ")
		rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	else if (facing == "positiveZ")
		rotation = glm::vec3(90.0f, 0.0f, 0.0f);
	else if (facing == "negativeX")
		rotation = glm::vec3(-90.0f, 90.0f, 0.0f);
	else if (facing == "positiveX")
		rotation = glm::vec3(90.0f, 90.0f, 0.0f);
	else if (facing == "negativeY")
		rotation = glm::vec3(0.0f, 0.0f, 180.0f);
	else if (facing == "positiveY") {
		rotation = glm::vec3(0.0f);
		issurround = true;
	}

	float tiling = (cullingBound / 2.0f) * 2.0f;
	
	objects.emplace_back(new Entity(enviModels.at(modelName), pos, scale, rotation, gridNo));
	objects.back()->setCullingBound(cullingBound + (cullingBound * 0.5f));
//	lights = lightingManager->getClosestLights(walls.back()->getPos());
	objects.back()->setPointLightIDs(p1, p2);
//	lights2 = lightingManager->getClosestSpotLights(walls.back()->getPos());
	objects.back()->setSpotLightIDs(s1, s2);
	objects.back()->setTiling(tiling);
}


bool EntityManager::writeFile() {
	// writing on a text file
	ofstream myfile("../Engenius/Levels/Level.dat");
	if (myfile.is_open())
	{
		myfile << "ModelName | Positions | Scale | Rotation | CullBound | GridNum | PointL1 | PointL2 \t//numberOfBodies on first line\n";
		myfile << entities.size() << "#\n";

		for (unsigned int objNo = 0; objNo < entities.size(); objNo++) {
			std::string modelName;
			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 rotation;

			modelName = entities.at(objNo)->getModel()->getName();
			position = entities.at(objNo)->getPos();
			scale = entities.at(objNo)->getScale();
			rotation = glm::vec3(entities.at(objNo)->getPitch(), entities.at(objNo)->getYaw(), entities.at(objNo)->getRoll());

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
			myfile << entities.at(objNo)->getCullingBound() << ",";
			myfile << entities.at(objNo)->getGridNo() << ",";
			myfile << entities.at(objNo)->getPointLightID(0) << ",";
			myfile << entities.at(objNo)->getPointLightID(1) << ",";
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
			int pointL1, pointL2;

			string asString;
			int variables = 14;
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
					case 11: gridNo = stoi(asString);	break;
					case 12: pointL1 = stoi(asString);	break;
					case 13: pointL2 = stoi(asString);	break;
					}
				}//for loop
				bodyNo++;
				addEntityObject(modelName, position, scale, rotation, cullingBound, gridNo, pointL1, pointL2, -1, -1);

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

void EntityManager::initLinkBetweenGrids() {
	//link rights
	addEntityWall("surround", glm::vec3(5.5f, 0.0f, -26.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(5.5f, 4.0f, -26.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(8.5f, 0.0f, -26.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(8.5f, 4.0f, -26.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 0.0f, -26.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 4.0f, -26.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 0.0f, -29.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 4.0f, -29.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 0.0f, -32.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 4.0f, -32.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 0.0f, -35.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(11.5f, 4.0f, -35.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(8.5f, 0.0f, -35.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(8.5f, 4.0f, -35.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(5.5f, 0.0f, -35.5f), glm::vec3(1.5f, 1.0f, 1.5f), "positiveY", 1.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(5.5f, 4.0f, -35.5f), glm::vec3(1.5f, 1.0f, 1.5f), "negativeY", 1.5f, grid_corridor, 2, -1, -1, -1);

	addEntityWall("surround", glm::vec3(8.5f, 2.0f, -37.0f), glm::vec3(4.5f, 1.0f, 4.5f), "positiveZ", 4.5f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(7.0f, 2.0f, -34.0f), glm::vec3(3.0f, 1.0f, 3.0f), "negativeZ", 3.0f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(13.0f, 2.0f, -34.0f), glm::vec3(3.0f, 1.0f, 3.0f), "negativeX", 3.0f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(13.0f, 2.0f, -28.0f), glm::vec3(3.0f, 1.0f, 3.0f), "negativeX", 3.0f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(10.0f, 2.0f, -31.0f), glm::vec3(3.0f, 1.0f, 3.0f), "positiveX", 3.0f, grid_corridor, 2, -1, -1, -1);
	addEntityWall("surround", glm::vec3(7.0f, 2.0f, -28.0f), glm::vec3(3.0f, 1.0f, 3.0f), "positiveZ", 3.0f, grid_corridor, 1, -1, -1, -1);
	addEntityWall("surround", glm::vec3(8.5f, 2.0f, -25.0f), glm::vec3(4.5f, 1.0f, 4.5f), "negativeZ", 4.5f, grid_corridor, 1, -1, -1, -1);
}

void EntityManager::initGeneratorRoom() {
	//generator
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -29.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeZ", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, -31.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(4.0f, 2.0f, -31.5f), glm::vec3(2.5f, 1.0f, 2.5f), "negativeX", 2.5f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -33.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, -33.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -41.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, -41.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -49.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, -49.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 2.0f, -53.0f), glm::vec3(8.0f, 1.0f, 8.0f), "positiveZ", 8.0f, grid_generator, 2, 3, -1, -1);//end

	//left
	addEntityWall("surround", glm::vec3(-8.0f, 2.0f, -51.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 0.0f, -51.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 4.0f, -51.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -49.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_generator, 2, 3, -1, -1);

	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -45.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-8.0f, 2.0f, -43.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 0.0f, -43.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 4.0f, -43.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -41.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -37.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_generator, 2, 3, -1, -1);

	//lightkey room
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -33.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 0.0f, -35.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 4.0f, -35.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(-8.0f, 2.0f, -35.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_generator, 2, 3, -1, -1);

	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, -47.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_generator, 2, 3, -1, -1);//out
	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, -39.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_generator, 2, 3, -1, -1);//out

	//right
	addEntityWall("surround", glm::vec3(8.0f, 2.0f, -51.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 0.0f, -51.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 4.0f, -51.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -49.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_generator, 2, 3, -1, -1);

	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -45.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(8.0f, 2.0f, -43.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 0.0f, -43.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 4.0f, -43.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_generator, 2, 3, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -41.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_generator, 2, 3, -1, -1);

	addEntityWall("surround", glm::vec3(4.0f, 2.0f, -47.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_generator, 2, 3, -1, -1);//out
	addEntityWall("surround", glm::vec3(4.0f, 2.0f, -39.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_generator, 2, 3, -1, -1);//out
}

void EntityManager::initLabBuilding() {
	//entrance
	addEntityWall("surround", glm::vec3(-2.0f, 2.0f, 4.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_entrance, 0, 1, -1, -1);//start
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -8.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, -8.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -16.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 0.0f, -24.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, -16.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(0.0f, 4.0f, -24.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(0.0f, 2.0f, -28.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveZ", 4.0f, grid_entrance, 0, 1, -1, -1);//end
	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, -24.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveX", 4.0f, grid_entrance, 0, 1, -1, -1);//left end
	addEntityWall("surround", glm::vec3(4.0f, 2.0f, -22.5f), glm::vec3(2.5f, 1.0f, 2.5f), "negativeX", 2.5f, grid_entrance, 0, 1, -1, -1);//right end

	//left rooms
	addEntityWall("surround", glm::vec3(-8.0f, 2.0f, -18.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 0.0f, -18.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 4.0f, -18.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -20.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -16.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, -14.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -12.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 2.0f, -4.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(-8.0f, 2.0f, -10.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-8.0f, 2.0f, -6.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 0.0f, -8.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-6.0f, 4.0f, -8.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, -2.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(-4.0f, 2.0f, 2.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveX", 2.0f, grid_entrance, 0, 1, -1, -1);

	//right rooms
	addEntityWall("surround", glm::vec3(8.0f, 2.0f, -18.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 0.0f, -18.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveY", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 4.0f, -18.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeY", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -20.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -16.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(4.0f, 2.0f, -14.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -12.0f), glm::vec3(2.0f, 1.0f, 2.0f), "positiveZ", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 2.0f, -4.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeZ", 2.0f, grid_entrance, 0, 1, -1, -1);
	
	addEntityWall("surround", glm::vec3(8.0f, 2.0f, -10.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(8.0f, 2.0f, -6.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 0.0f, -8.0f), glm::vec3(4.0f, 1.0f, 4.0f), "positiveY", 4.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(6.0f, 4.0f, -8.0f), glm::vec3(4.0f, 1.0f, 4.0f), "negativeY", 4.0f, grid_entrance, 0, 1, -1, -1);

	addEntityWall("surround", glm::vec3(4.0f, 2.0f, -2.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_entrance, 0, 1, -1, -1);
	addEntityWall("surround", glm::vec3(4.0f, 2.0f, 2.0f), glm::vec3(2.0f, 1.0f, 2.0f), "negativeX", 2.0f, grid_entrance, 0, 1, -1, -1);

	initLinkBetweenGrids();
	initGeneratorRoom();
}

void EntityManager::initObjectsToWorld() {
	readFile();

	initLabBuilding();
	objects.resize(objects.size());
	entities.resize(entities.size());
	//objects_disableCullface.resize(objects.size());

	//particleManager->insertSmoke(1, glm::vec3(0.0f, 2.0f, 0.0f), true);
	//particleManager->insertSmoke(1, glm::vec3(1.0f, 2.0f, 0.0f), true);
	//particleManager->insertSmoke(1, glm::vec3(-1.0f, 2.0f, 0.0f), true);
	//particleManager->insertSmoke(1, glm::vec3(-1.0f, 2.0f, 0.0f), true);
	//particleManager->insertSmoke(1, glm::vec3(-1.0f, 2.0f, 0.0f), true);
	//particleManager->insertFire(1, glm::vec3(0.0f, 4.25f, 37.5f), true);
	//particleManager->insertFire(1, glm::vec3(1.0f, 4.25f, 37.5f), true);
	//particleManager->insertFire(1, glm::vec3(-1.0f, 4.25f, 37.5f), true);
	//particleManager->insertFire(1, glm::vec3(-1.0f, 4.25f, 38.5f), true);
	//particleManager->insertFire(1, glm::vec3(-1.0f, 4.25f, 36.5f), true);
}

void EntityManager::initPlayer() {
	glm::vec3 pos(-2.0f, 0.0f, 2.0f);
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(pos.x, pos.y, pos.z));
	float rad = 0.75f;
	float height = 0.5f;
	float mass = 80.0f;
	btCapsuleShape* playerShape = new btCapsuleShape(rad, height);
	btVector3 inertia(0, 0, 0);

	if (mass != 0.0)
		playerShape->calculateLocalInertia(mass, inertia);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, playerShape, inertia);

	btRigidBody* playerBody = new btRigidBody(info);
	playerBody->setAngularFactor(0); // Doesn't fall sideways
	playerBody->setSleepingThresholds(0.0, 0.0);
	int playerCollidesWith = COL_DEFAULT | COL_TRIGGER;
	// body, group, mask
	colManager->addToWorld(playerBody, COL_PLAYER, playerCollidesWith);
	playerBody->setActivationState(DISABLE_DEACTIVATION);
	playerBody->setFriction(static_cast<btScalar>(0.0f));

	// Now ghost
	btPairCachingGhostObject* playerGhost = new btPairCachingGhostObject();								// create object
	playerGhost->setCollisionShape(playerShape);								// set shape
	playerGhost->setWorldTransform(t);											// set world transform	
	playerGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);  // disable collision response // could also add CF_CHARACTER_OBJECT // If I knew what that flag did...

	colManager->addGhostToWorld(playerGhost, COL_PLAYER, playerCollidesWith);

	player = new Character(charModels.at("cube"), pos, glm::vec3(0.5f, 1.0f, 0.5f), 0.0f, playerBody, playerGhost);
	std::pair<unsigned int, unsigned int> lights = lightingManager->getClosestLights(player->getPos());
	player->setPointLightIDs(lights.first, lights.second);
	lights = lightingManager->getClosestSpotLights(player->getPos());
	player->setSpotLightIDs(lights.first, lights.second);
}

EntityManager::EntityManager(Camera *camera, ParticleManager* particleManager, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager* colManager, TerrainManager* terrainManager, AudioManager * audioManager) : camera(camera), particleManager(particleManager), lightingManager(lightingManager), shaderManager(shaderManager), colManager(colManager), terrainManager(terrainManager), audioManager(audioManager){

	frustumCulling = new FrustumCulling();
	frustumCulling->setCamInternals(camera->getFOV() / DEG_TO_RADIAN, camera->getRatio(), camera->getNearPlane(), camera->getFarPlane()); //divide to send as degree
	
	initEnvironmentModels(enviModels);
	initCharacterModels(charModels);

	colManager->addCollisionModel(enviModels.at("cube"));

	initObjectsToWorld();
//CHECK FOR THE OBJECTS AFFECTED BY LIGHT HERE!!!!
	
	test = false;

	pause = true;
	endGame = false;

	controlReset_1 = false;
	controlReset_2 = false;
//	alarmStopped = false;
	previousTriggerNo = 0;

	initPlayer();
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
	srand(time(NULL));

	/* generate secret number between 0 and 4: */
	puzzle_firstTime.steps = pool_of_steps1[rand() % 4];
	puzzle_secondTime.steps = pool_of_steps2[rand() % 4];

	std::cout << "1st: " << puzzle_firstTime.steps.x << puzzle_firstTime.steps.y << puzzle_firstTime.steps.z << puzzle_firstTime.steps.w << std::endl;
	std::cout << "1st: " << puzzle_secondTime.steps.x << puzzle_secondTime.steps.y << puzzle_secondTime.steps.z << puzzle_secondTime.steps.w << std::endl;
}

void EntityManager::renderEnvironment() {
	GLuint shader = shaderManager->get_mapped_model_program();
	glUseProgram(shader);
	camera->passViewProjToShader(shader);
	farPlane_camEye_toShader(shader);
	lightingManager->lightsToShader(shader);
	if (lightingManager->getIfShadow() == true)
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), true);
	else
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), false);

	enviModels.at("surround")->bindWall(shader);
	for (unsigned int iter = 0; iter < objects.size(); iter++) {
		if (frustumCulling->sphereInFrustum(objects.at(iter)->getPos(), objects.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (objects.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = objects.at(iter)->getModel();
					const glm::mat4 objMatrix = objects.at(iter)->getModelMatrix();
					lightIDsToShader(shader, objects[iter]->getPointLightID(0), objects[iter]->getPointLightID(1), objects[iter]->getSpotLightID(0), objects[iter]->getSpotLightID(1));
					glUniform1f(glGetUniformLocation(shader, "tiling"), objects.at(iter)->getTiling());
					objModel->drawWall(shader, objMatrix);
					rendered = true;
				}
			}
		}
	}
	enviModels.at("surround")->unbindWall();
}

void EntityManager::renderEntities() {
	GLuint shader = shaderManager->get_mapped_model_program();
	glUseProgram(shader);
	camera->passViewProjToShader(shader);
	farPlane_camEye_toShader(shader);
	lightingManager->lightsToShader(shader);
	if (lightingManager->getIfShadow() == true)
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), true);
	else
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), false);

	for (unsigned int iter = 0; iter < entities.size(); iter++) {
		if (frustumCulling->sphereInFrustum(entities.at(iter)->getPos(), entities.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (entities.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = entities.at(iter)->getModel();
					const glm::mat4 objMatrix = entities.at(iter)->getModelMatrix();
					lightIDsToShader(shader, entities[iter]->getPointLightID(0), entities[iter]->getPointLightID(1), entities[iter]->getSpotLightID(0), entities[iter]->getSpotLightID(1));
					glUniform1f(glGetUniformLocation(shader, "tiling"), entities.at(iter)->getTiling());
					objModel->Draw(shader, objMatrix);
					rendered = true;
				}
			}
		}
	}
}

void EntityManager::renderCharacters() {
	//Player
	GLuint shader = shaderManager->get_animated_model_program();
	glUseProgram(shader);
	if (lightingManager->getIfShadow() == true)
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), true);
	else
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), false);
	farPlane_camEye_toShader(shader);
	camera->passViewProjToShader(shader);
	lightingManager->lightsToShader(shader);
	lightIDsToShader(shader, player->getPointLightID(0), player->getPointLightID(1), player->getSpotLightID(0), player->getSpotLightID(1));

	player->draw(shader);
}

void EntityManager::lightIDsToShader(GLuint shader, int pointLights_id1, int pointLights_id2, int spotLights_id1, int spotLights_id2) {
	lightingManager->lightIDsToShader(shader, pointLights_id1, pointLights_id2, spotLights_id1, spotLights_id2);
	if (lightingManager->getIfShadow() == true) {
		lightingManager->shadowMapsToShader(shader, pointLights_id1, pointLights_id2, spotLights_id1, spotLights_id2);
	}
	else {
		lightingManager->noShadowMessage(shader);
	}
}

void EntityManager::farPlane_camEye_toShader(GLuint shader) {
	glUniform1f(glGetUniformLocation(shader, "far_plane"), camera->getFarPlane());
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera->getCameraEye()));
}

void EntityManager::shadow_draw(GLuint shader) {
	farPlane_camEye_toShader(shader);

	//TERRAINS
//	terrainManager->shadowDraw(shader);

	//OBJECTS
	glDisable(GL_CULL_FACE);

	for (unsigned int iter = 0; iter < entities.size(); iter++) {
		if (frustumCulling->sphereInFrustum(entities.at(iter)->getPos(), entities.at(iter)->getCullingBound()) != 0) {
			bool rendered = false;
			for (int i = 0; i < NUM_EFFECTIVE_GRIDS; i++) {
				if (entities.at(iter)->getGridNo() == renderGridNo[i] && rendered == false) {
					Model *objModel = entities.at(iter)->getModel();
					const glm::mat4 objMatrix = entities.at(iter)->getModelMatrix();
				
				//	lightIDsToShader(shader, entities[iter]->getPointLightID(0), entities[iter]->getPointLightID(1), entities[iter]->getSpotLightID(0), entities[iter]->getSpotLightID(1));
				//	glUniform1f(glGetUniformLocation(shader, "tiling"), entities.at(iter)->getTiling());
					objModel->Draw(shader, objMatrix);
					rendered = true;
				}
			}
		}
	}

	glEnable(GL_CULL_FACE);
	//PLAYER
	//player->draw(shader);
}

void EntityManager::draw() {
	//terrainManager->render(test, player->getPos());
	renderEntities();
	renderEnvironment();
	//renderCharacters();	

}

void EntityManager::areaControl() {
	unsigned int grid = colManager->getRelevantGrid(player->getPos(), player->getScale());
	if (grid == 1) {
		renderGridNo[0] = grid_entrance;
		renderGridNo[1] = grid_corridor;
	}
	else if (grid == 2) {
		renderGridNo[0] = grid_generator;
		renderGridNo[1] = grid_corridor;
	}
}

void EntityManager::resetPuzzle(Puzzle *puzzle) {
	puzzle->gotFirst = false;
	puzzle->gotSecond = false;
	puzzle->gotThird = false;
}

void EntityManager::checkStep(Puzzle *puzzle, int triggerNo) {
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
	else if (triggerNo != previousTriggerNo){
		audioManager->playSFX_wrong(camera->getCameraEye());
		resetPuzzle(puzzle);
		previousTriggerNo = 0;
	}
}

void EntityManager::checkTrigger() {
	unsigned int grid = colManager->getRelevantGrid(player->getPos(), player->getScale());
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
			lightingManager->setExposure(2.0f);
		}
	}
}

void EntityManager::Update(const float dt_secs) {
	frustumCulling->setCamDef(camera->getCameraEye(), camera->getCameraDirection(), camera->getCameraUp());

	float terrHeightAtPlayerPos = 0.0f;//terrainManager->getTerrainHeight(player->getPos().x, player->getPos().z);

	colManager->update();
	player->Update(camera->getYaw(), camera->getCameraAt(), camera->getCameraUp(), terrHeightAtPlayerPos, dt_secs);
	audioManager->updateListenerPosition(camera->getCameraEye());

	areaControl();
}

bool EntityManager::getIfEndGame() {
	return endGame;
}
bool EntityManager::getIfPause() {
	return pause;
}
void EntityManager::setPause(bool newVal) {
	pause = newVal;
}

Entity* EntityManager::getObject(unsigned int i) {	return objects.at(i); }

Character* EntityManager::getPlayer() { return this->player; }
Model* EntityManager::getENVModel(const std::string name) { return this->enviModels.at(name); }

void EntityManager::setEntityPos(const glm::vec3 pos, int indexOfSelectedObject) {
	objects.at(indexOfSelectedObject)->setPos(pos);
	glm::mat4 model;
	Common::createModelMatrix(model, pos, objects.at(indexOfSelectedObject)->getYaw(), objects.at(indexOfSelectedObject)->getScale());
	objects.at(indexOfSelectedObject)->setModelMatrix(model);
}

std::unordered_map<std::string, Model*>::iterator EntityManager::getEnviModel_Begin() {	return enviModels.begin(); }
std::unordered_map<std::string, Model*>::iterator EntityManager::getEnviModel_End() { return enviModels.end(); }
std::vector<Entity*>::iterator EntityManager::getEnviObject_Begin() { return objects.begin(); }
std::vector<Entity*>::iterator EntityManager::getEnviObject_End() { return objects.end(); }