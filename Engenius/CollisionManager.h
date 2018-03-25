
#ifndef COLLISIONMANAGER
#define COLLISIONMANAGER

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include "Model.h"
#include "Common.h"
#include <map>
#include <fstream>

#include <btBulletDynamicsCommon.h>

#define GRAVITY -10

// TODO:move
#define BIT(x) (1<<(x))
enum collisiontype {
	COL_NOTHING = 0,
	COL_DEFAULT = BIT(0),
	COL_PLAYER = BIT(1),
	COL_TRIGGER = BIT(2),
};
#define COLLIDE_ALL COL_DEFAULT | COL_PLAYER | COL_TRIGGER
//!!

// Settings struct
struct btSettings {
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase; //improves collision check (?) // can improve by knowing world size (?)
	btConstraintSolver* solver;
};

struct HeirarchicalGrid {
	glm::vec2 position, scale;
	float left, right, top, bottom;

	HeirarchicalGrid() { ; }
	HeirarchicalGrid(glm::vec2 pos, glm::vec2 scale) : position(pos), scale(scale) { ; }
};

class CollisionManager {
public:
	CollisionManager();
	void addCollisionModel(Model* cube);

	void addHeirarchicalGrid(glm::vec2 pos, glm::vec2 scale);
	unsigned int getRelevantGrid(const glm::vec3 pos, const glm::vec3 scale);

	bool readBoundFile();
	bool writeBoundFile();

	void renderBoundingBoxes(GLuint shader, bool addMode, bool rigidBodyMode, glm::mat4 edit_modelMatrix);

	float getHeirarchicalGridScale();

	std::vector<btRigidBody*>::iterator getRigidBegin();
	std::vector<btRigidBody*>::iterator getRigidEnd();

	std::vector<HeirarchicalGrid*>::iterator getHeirBegin();
	std::vector<HeirarchicalGrid*>::iterator getHeirEnd();

	void update();
	void displayGridDetails();

	btRigidBody* addBox(float width, float height, float depth, float x, float y, float z, float mass);
	btRigidBody* addBox(float width, float height, float depth, float x, float y, float z, float mass, collisiontype group, collisiontype mask);
	btRigidBody* addCapsule(float rad, float height, float x, float y, float z, float mass);

	void renderBox(btRigidBody* box, GLuint shader);
	void renderCapsule(btRigidBody* capsule, Model *modelData, GLuint shader, GLuint texture);
	void addToWorld(btRigidBody* body);
	void addToWorld(btRigidBody* body, collisiontype COLL_TYPE, int collidesWith);
	void addGhostToWorld(btPairCachingGhostObject* ghost);
	void addGhostToWorld(btPairCachingGhostObject* ghost, collisiontype COLL_TYPE, int collidesWith);
	btBroadphasePair* findWorldPair(const btBroadphasePair &pair);
	void removeObject(btRigidBody* body);
	void removeObject(btPairCachingGhostObject* ghost);

private:
	bool heirDisplay;

	void initGrids();
	
	bool twoDcollisionCheck(const glm::vec2 position_A, const glm::vec2 scale_A, const float left, float right, const float top, const float bottom);

	Model* cube;
	const glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	std::map<std::string, std::vector<HeirarchicalGrid*>*> grids;
	std::vector<btRigidBody*> rigidBodies;

	btSettings btSettings;
};

#endif