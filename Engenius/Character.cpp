#include "Character.h"
//////CHANGE THE GRID AND AREA NO OR ELSE CHARACTER WONT SHOW
Character::Character(AnimatedModel* model, const glm::vec3& position, const glm::vec3& scale, const float& yaw) : model(model), Entity(model, position, scale, glm::vec3(0.0f, yaw, 0.0f), 0) {
	speed = 0.0f;
	strafeSpeed = 0.0f;
	upwardsSpeed = 0.0f;
	this->walkSpeedLimit = MAX_SPEED * 0.35f;
	this->currentState = 0;
	transforms.resize(model->getNumBones());
	animated = true;
	currentState = IDLE_STATE;
	setCullingBound(5.0f);
}

void Character::addCollider(btRigidBody* bounding, btPairCachingGhostObject* ghost) {
	this->bounding = bounding;
	this->ghost = ghost;
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

static btVector3 getLinearVelocityInBodyFrame(const btRigidBody* body)
{
	return(body->getWorldTransform().getBasis().transpose() *
		body->getLinearVelocity());
}

static btVector3 convertglm2bt(const glm::vec3& toCon) {
	return btVector3(toCon.x, toCon.y, toCon.z);
}

void Character::Update(const float& yaw, const glm::vec3& cameraAt, const glm::vec3& cameraUp, const float& terrainHeight, const float& dt_secs) {
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

	model->BoneTransform(dt_secs, transforms);
}

void Character::draw(const unsigned int& shader, Renderer* renderer) {
	if (animated) {
		for (unsigned int i = 0; i < transforms.size(); i++) {
			const unsigned int MAX_BONES = 50;
			assert(i < MAX_BONES);
			unsigned int uniformIndex = glGetUniformLocation(shader, ("boneLocation[" + to_string(i) + "]").c_str());
			glUniformMatrix4fv(uniformIndex, 1, GL_TRUE, (const GLfloat*)transforms.at(i));
		}
	}
	glm::mat4 modelMatrix;
	//Common::createModelMatrix(modelMatrix, position, rotation, scale);
	Common::createModelMatrix(modelMatrix, glm::vec3(-1.0f, 0.5f, 8.5f), scale, rotation);
	glUniform1i(glGetUniformLocation(shader, "animated"), animated);
	model->Draw(shader, modelMatrix, renderer);
}

void Character::changeAnimation() {
	if (currentState == IDLE_STATE) {
		currentState = WALK_STATE;
		model->walkAnimation();
	}
	else if (currentState == WALK_STATE) {
		currentState = JOG_STATE;
		model->jogAnimation();
	}
	else if (currentState == JOG_STATE) {
		currentState = WALK_STATE;
		model->walkAnimation();
	}
	//model->idleAnimation();
}

void Character::setAnimated(const bool& newValue) {
	animated = newValue;
}

btRigidBody* Character::getBounding() {
	return bounding;
}

AnimatedModel* Character::getModel() { 
	return this->model; 
}