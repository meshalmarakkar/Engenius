#include "ModelManager.h"

ModelManager::ModelManager() {
	initModels_Envi();
	initModels_Char();
}

ModelManager::~ModelManager() {
	for (std::unordered_map<std::string, Model*>::iterator it = models_envi.begin(); it != models_envi.end(); it++) {
		delete &it;
	}

	for (std::unordered_map<std::string, AnimatedModel*>::iterator it = models_char.begin(); it != models_char.end(); it++) {
		delete &it;
	}
}

Model* ModelManager::getModel_envi(const std::string& storageName) {
	return models_envi.at(storageName);
}
AnimatedModel* ModelManager::getModel_char(const std::string& storageName) {
	return models_char.at(storageName);
}

std::unordered_map<std::string, Model*>::iterator ModelManager::getEnviModel_Begin() { return models_envi.begin(); }
std::unordered_map<std::string, Model*>::iterator ModelManager::getEnviModel_End() { return models_envi.end(); }

void ModelManager::initModels_Envi() {
	const string envDirectory = "../Engenius/Models/Environment/";
	string path = "";

	//cout << "wall -> ";
	path = envDirectory + "Terrain/plane.obj";
	models_envi.insert(std::pair<std::string, Model*>("surround", new Model("surround", path, "whiteTile/diffuse.png", "whiteTile/specular.png", "whiteTile/normal.png")));

	//cout << "cube -> ";
	path = envDirectory + "Terrain/cube.obj";
	models_envi.insert(std::pair<std::string, Model*>("cube", new Model("cube", path, "whiteTile/diffuse.png", "whiteTile/specular.png", "whiteTile/normal.png")));

	//cout << "ceilingLight -> ";
	path = envDirectory + "light/ceilingLight.obj";
	models_envi.insert(std::pair<std::string, Model*>("ceilingLight", new Model("ceilingLight", path, "diffuse.png", NULL, "normal.png")));

	//	cout << "computer -> ";
	path = envDirectory + "Computer_Laptop/Computer_Laptop.obj";
	models_envi.insert(std::pair<std::string, Model*>("computer", new Model("computer", path)));

	//	cout << "securityDoor -> ";
	path = envDirectory + "Door/big_security_door.obj";
	models_envi.insert(std::pair<std::string, Model*>("securityDoor", new Model("securityDoor", path, "door_cell_d.png", NULL, "door_cell_n.png")));

	//	cout << "controlPanel -> ";
	path = envDirectory + "Control Panel/Machine 08 Straight.obj";
	models_envi.insert(std::pair<std::string, Model*>("controlPanel", new Model("controlPanel", path, "ABY_Machine_08_A_Diffuse.png", NULL, "ABY_Machine_08_A_NormalsMap.png")));

	//	cout << "electronicDevice -> ";
	path = envDirectory + "ElectronicDevice/hacking tool.obj";
	models_envi.insert(std::pair<std::string, Model*>("electronicDevice", new Model("electronicDevice", path)));

	//	cout << "closedLocker -> ";
	path = envDirectory + "Cabinet_non-open/armory_cabinet.obj";
	models_envi.insert(std::pair<std::string, Model*>("closedLocker", new Model("closedLocker", path, "armory_cabinet_d.png", "armory_cabinet_m2.png", "armory_cabinet_n.png")));

	//	cout << "wallTech -> ";
	path = envDirectory + "WallTech/Wall_Tech_M.obj";
	models_envi.insert(std::pair<std::string, Model*>("wallTech", new Model("wallTech", path, "WallTech_Diffuse.png", NULL, "WallTech_NormalMap.png")));

	//	cout << "speaker -> ";
	path = envDirectory + "Speaker/Speaker.obj";
	models_envi.insert(std::pair<std::string, Model*>("speaker", new Model("speaker", path)));

	//	cout << "server -> ";
	path = envDirectory + "Server/ServerV2+console.obj";
	models_envi.insert(std::pair<std::string, Model*>("server", new Model("server", path)));

	//	cout << "desk -> ";
	path = envDirectory + "BasicDesk/Desk.obj";
	models_envi.insert(std::pair<std::string, Model*>("desk", new Model("desk", path, "diffuse.jpg", "specular.jpg", "normal.jpg")));
}

void ModelManager::initModels_Char() {
	const string charDirectory = "../Engenius/Models/Characters/";
	string path = "";

	cout << "steamPunkBatman -> ";
	path = charDirectory + "Batman/warrior_idle.dae";
	models_char.insert(std::pair<std::string, AnimatedModel*>("batman", new AnimatedModel("batman", path)));
	path = charDirectory + "Batman/walking.dae";
	models_char.insert(std::pair<std::string, AnimatedModel*>("batmanWalk", new AnimatedModel(path, true)));
	path = charDirectory + "Batman/running.dae";
	models_char.insert(std::pair<std::string, AnimatedModel*>("batmanJog", new AnimatedModel(path, true)));
	path = charDirectory + "Batman/standard_run.dae";
	models_char.insert(std::pair<std::string, AnimatedModel*>("batmanRun", new AnimatedModel(path, true)));
	path = charDirectory + "Batman/walking_backwards.dae";
	models_char.insert(std::pair<std::string, AnimatedModel*>("batmanWalkBack", new AnimatedModel(path, true)));
	models_char.at("batman")->setMovementAnims(models_char.at("batmanWalk")->getAnims());
	models_char.at("batman")->setMovementAnims(models_char.at("batmanJog")->getAnims());
	models_char.at("batman")->setMovementAnims(models_char.at("batmanRun")->getAnims());
	models_char.at("batman")->setMovementAnims(models_char.at("batmanWalkBack")->getAnims());
	models_char.erase("batmanWalk");
	models_char.erase("batmanJog");
	models_char.erase("batmanRun");
	models_char.erase("batmanWalkBack");
}