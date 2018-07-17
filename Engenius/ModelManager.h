//#pragma once
#ifndef MODELMANAGER
#define MODELMANAGER

#include <unordered_map>
#include "AnimatedModel.h"
#include "Model.h"
#include <string>

class ModelManager {
public:
	ModelManager();
	~ModelManager();

	Model* getModel_envi(const std::string& storageName);
	AnimatedModel* getModel_char(const std::string& storageName);

	std::unordered_map<std::string, Model*>::iterator getEnviModel_Begin();
	std::unordered_map<std::string, Model*>::iterator getEnviModel_End();

private:
	void initModels_Envi();
	void initModels_Char();

private:
	std::unordered_map<std::string, AnimatedModel*> models_char;
	std::unordered_map<std::string, Model*> models_envi;
};

#endif