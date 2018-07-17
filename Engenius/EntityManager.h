#ifndef ENTITYMANAGER
#define ENTITYMANAGER

#include "Model.h"
#include "AnimatedModel.h"
#include "Renderer.h"
#include "Entity.h"
#include "Character.h"
#include "Entity.h"
#include "CollisionManager.h"
#include "LightingManager.h"
#include "Culling.h"
#include "Camera.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <unordered_map>
#include "TerrainManager.h"
#include "ModelManager.h"

class EntityManager {
public:
	EntityManager(Camera *camera, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager * colManager, TerrainManager* terrainManager);

	void draw(const float& dt_secs, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);
	void shadow_draw(const unsigned int& shader, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[]);
	void Update(const float& dt_secs);

	Character* getPlayer();
	Model* getENVModel(const std::string& name);
	void addEntityObject(const std::string& mdlName, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, const float& culBound, const unsigned int& gridNo, const int& p1, const int& p2, const int& p3, const int& s1, const int& s2, const int& s3);
	void farPlane_camEye_toShader(const unsigned int& shader);

	Entity* getObject(const unsigned int& i);
	bool writeFile();
	bool writeFile_Panels();
	void changeAnimation();

	void setEntityPos(const glm::vec3& pos, const int& indexOfSelectedObject);
	std::unordered_map<std::string, Model*>::iterator getEnviModel_Begin();
	std::unordered_map<std::string, Model*>::iterator getEnviModel_End();
	std::vector<Entity*>::iterator getEnviObject_Begin();
	std::vector<Entity*>::iterator getEnviObject_End();

	int tempnum = 0;
	void increNum() {
		tempnum++;
		if (tempnum > 7)
			tempnum = 0;
	}
	int getNum() {
		return tempnum;
	}

private:
	Camera* camera;
	FrustumCulling* frustumCulling;
	LightingManager* lightingManager;
	ShaderManager* shaderManager;
	Character* player;
	TerrainManager* terrainManager;
	ModelManager* modelManager;

	void initPlayer(CollisionManager* colManager);
	bool readFile();
	bool readFile_Panels();

	void initObjectsToWorld();
	void lightIDsToShader(const unsigned int& shader, const int& point_id1, const int& point_id2, const int& point_id3, const int& spot_id1, const int& spot_id2, const int& spot_id3);
	void renderPanels(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);
	void renderObjects(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);
	void renderCharacters(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);

	std::vector<Entity*> panels;
	std::vector<Entity*> objects;

	void addEntityPanels(const std::string& mdlName, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, const float& culBound, const unsigned int& gridNo, const int& p1, const int& p2, const int& p3, const int& s1, const int& s2, const int& s3);

//	std::unordered_map<std::string, AnimatedModel*> charModels;
//	std::unordered_map<std::string, Model*> enviModels;

	Renderer* renderer;
};

#endif