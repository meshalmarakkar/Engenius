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
//#include <tuple>

class EntityManager {
public:
	EntityManager(Camera *camera, ParticleManager* particleManager, LightingManager* lightingManager, ShaderManager* shaderManager, CollisionManager * colManager, TerrainManager* terrainManager, AudioManager* audioManager);

	void draw(float dt_secs);
	void draw_GBuffer();
	void shadow_draw(GLuint shader);
	void Update(const float dt_secs);
	Character* getPlayer();
	Model* getENVModel(const std::string name);
	void addEntityObject(std::string mdlName, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, float culBound, unsigned int gridNo, int p1, int p2, int p3, int s1, int s2, int s3);
	void farPlane_camEye_toShader(GLuint shader);

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

	bool getIfDef();
	void toggleDeferredShading();

private:
	bool deferredShading;

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
	void lightIDsToShader(GLuint shader, int point_id1, int point_id2, int point_id3, int spot_id1, int spot_id2, int spot_id3);
	void renderPanels();
	void renderObjects();
	void renderCharacters();
	void renderPanels_GBuffer();
	void renderObjects_GBuffer();
	void renderCharacters_GBuffer();
	
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

	void addEntityPanels(std::string mdlName, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, float culBound, unsigned int gridNo, int p1, int p2, int p3, int s1, int s2, int s3);
#define NUM_EFFECTIVE_GRIDS 2
	unsigned int renderGridNo[NUM_EFFECTIVE_GRIDS];
	
	void areaControl();

	std::unordered_map<std::string, AnimatedModel*> charModels;
	std::unordered_map<std::string, Model*> enviModels;

	ParticleManager *particleManager;
	bool test;
};

#endif