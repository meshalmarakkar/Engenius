#ifndef CHARACTER
#define CHARACTER

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "AnimatedModel.h"
#include "Entity.h"
#include "Common.h"
#include <btBulletDynamicsCommon.h>

class Character : public Entity {
public:
	Character(AnimatedModel* model, const glm::vec3& position, const glm::vec3& scale, const float& yaw);

	AnimatedModel* getModel();
	void setAnimated(const bool& newValue);
	void boneLocationToShader(const unsigned int& shader);
	void changeAnimation();

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void Update(const float& yaw, const glm::vec3& cameraAt, const glm::vec3& cameraUp, const float& terrainHeight, const float& dt_secs);

	btRigidBody* getBounding();
	void addCollider(btRigidBody* bounding, btPairCachingGhostObject* ghost);

private:
	glm::mat4 temp_character_display_model;

	AnimatedModel* model;
	std::vector<Matrix4f> transforms;
	bool animated;

	btRigidBody* bounding;
	btPairCachingGhostObject* ghost;

	const float offset = 90.0f;
	float speed;
	const float ACCELERATION = 80.0f;
	const float MAX_SPEED = 400.0f;
	const float MAX_REVERSE_SPEED = 400.0f;
	float strafeSpeed;
	const float STRAFE_ACCELERATION = 400.0f;
	const float STRAFE_MAX_SPEED = 80.0f;

	const float ZERO = 0.0f;
	const float FRICTION = 20.0f;
	const float TURN_SPEED = 15.0f;
	const float GRAVITY = 50.0f;
	const float JUMP_POWER = 30.0f;

	float upwardsSpeed;
	unsigned short currentState;
	const unsigned short IDLE_STATE = 0;
	const unsigned short WALK_STATE = 1;
	const unsigned short JOG_STATE = 2;
	const unsigned short RUN_STATE = 3;
	const unsigned short WALK_BACK_STATE = 3;
	float walkSpeedLimit;
	float jogSpeedLimit;
	float runSpeedLimit;
};

#endif