#ifndef ENTITYMANAGER
#define ENTITYMANAGER

#include "Model.h"
#include "AnimatedModel.h"
#include "Entity.h"
#include "Terrain.h"
#include "Character.h"
#include "Entity.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "LightingManager.h"
#include "Culling.h"
#include "Camera.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <unordered_map>
#include <btBulletDynamicsCommon.h>
#include "TerrainManager.h"
#include "AudioManager.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class EntityManager {
public:
	EntityManager(Camera *camera, ParticleManager* particleManager, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager * colManager, TerrainManager* terrainManager, AudioManager* audioManager);

	void draw();
	void shadow_draw(GLuint shader);
	void Update(const float dt_secs);
	Character* getPlayer();
	Model* getENVModel(const std::string name);
	void addEntityObject(std::string modelName, glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, float cullingBound, unsigned int gridNo, int p1, int p2, int s1, int s2);

	Entity* getObject(unsigned int i);
	void checkTrigger();
	bool writeFile();
	bool writeFile_Panels();
	void changeAnimation();

	void setEntityPos(const glm::vec3 pos, int indexOfSelectedObject);
	std::unordered_map<std::string, Model*>::iterator getEnviModel_Begin();
	std::unordered_map<std::string, Model*>::iterator getEnviModel_End();
	std::vector<Entity*>::iterator getEnviObject_Begin();
	std::vector<Entity*>::iterator getEnviObject_End();

	bool getIfEndGame();
	bool getIfPause();
	void setPause(bool newVal);

private:
	Camera* camera;
	CollisionManager* colManager;
	FrustumCulling* frustumCulling;
	LightingManager* lightingManager;
	ShaderManager* shaderManager;
	Character* player;
	TerrainManager* terrainManager;
	AudioManager* audioManager;

	void initPlayer();
	bool readFile();
	bool readFile_Panels();

	void initObjectsToWorld();
	void lightIDsToShader(GLuint shader, int pointLights_id1, int pointLights_id2, int spotLights_id1, int spotLights_id2);
	void farPlane_camEye_toShader(GLuint shader);
	void renderEnvironment();
	void renderObjects();
	void renderCharacters();
	
	std::vector<Entity*> panels;
	std::vector<Entity*> objects;

	const unsigned int grid_entrance = 0;
	const unsigned int grid_corridor = 1;
	const unsigned int grid_generator = 2;

	glm::vec4 pool_of_steps1[4];
	glm::vec4 pool_of_steps2[4];

	struct Puzzle {
		glm::vec4 steps;
		bool gotFirst;
		bool gotSecond;
		bool gotThird;
		bool solved;

		Puzzle() {
			gotFirst = false;
			gotSecond = false;
			gotThird = false;
			solved = false;
		}
	};

	int previousTriggerNo;
	
	bool controlReset_1;
	bool controlReset_2;
	//bool alarmStopped;

	bool endGame;
	bool pause;

	Puzzle puzzle_firstTime;
	Puzzle puzzle_secondTime;

	void resetPuzzle(Puzzle *puzzle);
	void checkStep(Puzzle *puzzle, int triggerNo);

	void addEntityPanels(std::string modelName, glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, float cullingBound, unsigned int gridNo, int p1, int p2, int s1, int s2);
#define NUM_EFFECTIVE_GRIDS 2
	unsigned int renderGridNo[NUM_EFFECTIVE_GRIDS];
	
	void areaControl();

	std::unordered_map<std::string, AnimatedModel*> charModels;
	std::unordered_map<std::string, Model*> enviModels;

	ParticleManager *particleManager;
	bool test;
};

#endif