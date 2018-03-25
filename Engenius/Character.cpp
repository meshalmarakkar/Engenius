#include "Character.h"
//////CHANGE THE GRID AND AREA NO OR ELSE CHARACTER WONT SHOW
Character::Character(AnimatedModel* model, glm::vec3 position, glm::vec3 scale, float yaw, btRigidBody* bounding, btPairCachingGhostObject* ghost) : model(model), Entity(model, position, scale, glm::vec3(0.0f, yaw, 0.0f), 0), bounding(bounding), ghost(ghost) {
	speed = 0.0f;
	strafeSpeed = 0.0f;
	upwardsSpeed = 0.0f;
	this->walkSpeedLimit = MAX_SPEED * 0.35f;
	this->currentState = 0;
	//transforms.resize(model->getNumBones());
	animated = false;
}

static btVector3 getLinearVelocityInBodyFrame(btRigidBody* body)
{
	return(body->getWorldTransform().getBasis().transpose() *
		body->getLinearVelocity());
}

btVector3 convertglm2bt(glm::vec3 toCon) {
	return btVector3(toCon.x, toCon.y, toCon.z);
}

void Character::MoveForward() {
	if (speed < MAX_SPEED) {
		speed += ACCELERATION;
	}
}
void Character::MoveBackward() {
	if (speed > -MAX_REVERSE_SPEED) {
		speed -= ACCELERATION;
	}
}
void Character::MoveLeft() {
	/*if (strafeSpeed > -STRAFE_MAX_SPEED) {
		strafeSpeed -= STRAFE_ACCELERATION;
	}*/
}
void Character::MoveRight() {
	/*if (strafeSpeed < STRAFE_MAX_SPEED) {
		strafeSpeed += STRAFE_ACCELERATION;
	}*/
}


AnimatedModel* Character::getModel() { return this->model; }

void Character::setAnimated(bool newValue) {
	animated = newValue;
}

void Character::Update(const float yaw, glm::vec3 cameraAt, glm::vec3 cameraUp, const float terrainHeight, const float dt_secs) {
//	this->rotation.y = this->initialYaw - yaw;

	if (speed != ZERO) {
		if (speed > ZERO) {
			speed -= FRICTION;

			/*if (speed >= walkSpeedLimit) {
				if (currentState != JOG_STATE) {
					model->jogAnimation();
					currentState = JOG_STATE;
				}
			}
			else if (currentState != WALK_STATE) {
				model->walkAnimation();
				currentState = WALK_STATE;
			}*/
		}
		else {
			speed += FRICTION;
			/*if (currentState != WALK_BACK_STATE) {
				model->walkBackAnimation();
				currentState = WALK_BACK_STATE;
			}*/
		}

		btScalar boundspeed = getLinearVelocityInBodyFrame(bounding).length() + speed;// +btVector3(1.0, 0, 1.0);
		bounding->setLinearVelocity(btVector3(cameraAt.x, 0, cameraAt.z) * boundspeed * dt_secs);
		//glm::vec3 forwardMovement(cameraAt * speed * dt_secs);
		//this->position += glm::vec3(forwardMovement.x, 0, forwardMovement.z);
		//this->rotation.y = this->initialYaw - yaw;
	}
	//else if (strafeSpeed != ZERO) {
	//	if (strafeSpeed > ZERO) {
	//		strafeSpeed -= FRICTION;

	//		if (strafeSpeed >= walkSpeedLimit) {
	//			if (currentState != IDLE_STATE) {
	//				model->jogAnimation();
	//				currentState = IDLE_STATE;
	//			}
	//		}
	//		else if (currentState != IDLE_STATE) {
	//			model->walkAnimation();
	//			currentState = IDLE_STATE;
	//		}
	//	}
	//	else {
	//		strafeSpeed += FRICTION;
	//		if (currentState != IDLE_STATE) {
	//			model->walkBackAnimation();
	//			currentState = IDLE_STATE;
	//		}
	//	}

	//	btScalar boundspeed = getLinearVelocityInBodyFrame(bounding).length() + strafeSpeed;// +btVector3(1.0, 0, 1.0);
	//	glm::vec3 sidewardMovement = glm::normalize(glm::cross(glm::vec3(cameraAt.x, 0, cameraAt.z), cameraUp));
	//	bounding->setLinearVelocity(btVector3(sidewardMovement.x, 0, sidewardMovement.z) * boundspeed * dt_secs);
	//	/*	glm::vec3 sidewardMovement(glm::normalize(glm::cross(cameraAt, cameraUp)) * strafeSpeed * dt_secs);
	//	this->position += glm::vec3(sidewardMovement.x, 0, sidewardMovement.z);*/
	//}
	//else {
	//	if (currentState != IDLE_STATE) {
	//		model->idleAnimation();
	//		currentState = IDLE_STATE;
	//	}
	//}

	if (this->position.y > terrainHeight) {
		upwardsSpeed -= GRAVITY * dt_secs;
		this->position.y += upwardsSpeed * dt_secs;
		//	bounding->setLinearVelocity(btVector3(getLinearVelocityInBodyFrame(bounding).x(), getLinearVelocityInBodyFrame(bounding).y() + (upwardsSpeed * dt_secs), getLinearVelocityInBodyFrame(bounding).z()));
	}
	//btScalar boundspeed = getLinearVelocityInBodyFrame(bounding).length() + upwardsSpeed;// +btVector3(1.0, 0, 1.0);
	//bounding->setLinearVelocity(btVector3(getLinearVelocityInBodyFrame(bounding).x(), getLinearVelocityInBodyFrame(bounding).y() + upwardsSpeed, getLinearVelocityInBodyFrame(bounding).z()));

	if (this->position.y < terrainHeight) {
		upwardsSpeed = 0.0f;
		btTransform t;
		t.setIdentity();
		bounding->getMotionState()->getWorldTransform(t);
		btVector3 pos = t.getOrigin();
		t.setOrigin(btVector3(pos.x(), terrainHeight, pos.z()));
		//	bounding->setWorldTransform(t);
		bounding->translate(btVector3(0, terrainHeight - pos.y(), 0));
		//bounding->setLinearVelocity(btVector3(getLinearVelocityInBodyFrame(bounding).x(), glm::abs(terrainHeight), getLinearVelocityInBodyFrame(bounding).z()));
		this->position.y = terrainHeight;
	}


	btTransform t;
	t.setIdentity();
	bounding->getMotionState()->getWorldTransform(t);
	btVector3 pos = t.getOrigin();
	//position = glm::vec3(pos.x(), pos.y(), pos.z());
	this->position.x = pos.x();
	this->position.z = pos.z();
	ghost->setWorldTransform(t);

//	model->BoneTransform(dt_secs, transforms);
}

btRigidBody* Character::getBounding() {
	return bounding;
}

void Character::draw(GLuint shader) {
	if (animated) {
		for (GLuint i = 0; i < transforms.size(); i++) {
			const GLuint MAX_BONES = 50;
			assert(i < MAX_BONES);
			GLuint uniformIndex = glGetUniformLocation(shader, ("boneLocation[" + to_string(i) + "]").c_str());
			glUniformMatrix4fv(uniformIndex, 1, GL_TRUE, (const GLfloat*)transforms.at(i));
		}
	}
	glm::mat4 modelMatrix;
	Common::createModelMatrix(modelMatrix, position, rotation, scale);
	glUniform1i(glGetUniformLocation(shader, "animated"), animated);
	model->Draw(shader, modelMatrix);
}