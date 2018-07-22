#ifndef ENTITYMANAGER
#define ENTITYMANAGER

#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

#include "Model.h"
#include "AnimatedModel.h"
#include "Renderer.h"
#include "Entity.h"
#include "Character.h"
#include "CollisionManager.h"
#include "LightingManager.h"
#include "Culling.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "TerrainManager.h"
#include "ModelManager.h"

class EntityManager {
public:
	EntityManager(Camera *camera, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager * colManager, TerrainManager* terrainManager, Renderer* renderer);

	void draw(const float& dt_secs, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);
	void shadow_draw(Shader* program, const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[]);
	void Update(Camera* camera, const float& dt_secs);

	void shadowTest();

	Character* getPlayer();
	Model* getENVModel(const std::string& name);
	void addEntityObject(const std::string& mdlName, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, const float& culBound, const unsigned int& gridNo, const int& p1, const int& p2, const int& p3, const int& s1, const int& s2, const int& s3, float tiling);
	
	Entity* getObject(const unsigned int& i);
	bool writeFile();
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
	FrustumCulling* frustumCulling;
	LightingManager* lightingManager;
	ShaderManager* shaderManager;
	Character* player;
	TerrainManager* terrainManager;
	ModelManager* modelManager;

	void initPlayer(CollisionManager* colManager);
	bool readFile();

	void lightIDsToShader(const unsigned int& shader, const glm::ivec3& pointLightIDs, const glm::ivec3& spotLightIDs);
	void renderObjects(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);
	void renderCharacters(const int& NUM_EFFECTIVE_GRIDS, const unsigned int renderGridNo[], const bool& ifDeferred);

	std::vector<Entity*> objects;

	Renderer* renderer;
};

#endif