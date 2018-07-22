#include "CollisionManager.h"

CollisionManager::CollisionManager() { 
	btSettings.collisionConfig = new btDefaultCollisionConfiguration();
	btSettings.dispatcher = new btCollisionDispatcher(btSettings.collisionConfig); //base algorithm good (?)
	btSettings.broadphase = new btDbvtBroadphase(); //divide space into different spaces // can use a more performancy ones // axis sweep?
	btSettings.solver = new btSequentialImpulseConstraintSolver(); // can use OpenCL, multithreading (?)
	btSettings.world = new btDiscreteDynamicsWorld(btSettings.dispatcher, btSettings.broadphase, btSettings.solver, btSettings.collisionConfig);
	btSettings.world->setGravity(btVector3(0, 0, 0)); // x y z as usual

	readBoundFile();

	std::vector<HeirarchicalGrid*> * grid = new std::vector<HeirarchicalGrid*>;
	grids.insert(std::pair<std::string, std::vector<HeirarchicalGrid*>*>("level", grid));
	initGrids();
}

void CollisionManager::addCollisionModel(Model* cube) {
	this->cube = cube;
}

void CollisionManager::initGrids() {
	addHeirarchicalGrid(glm::vec2(11.5f, -30.0f), glm::vec2(1.6f, 0.2f));
	addHeirarchicalGrid(glm::vec2(11.5f, -32.0f), glm::vec2(1.6f, 0.2f));

	addHeirarchicalGrid(glm::vec2(-3.2f, 1.35f), glm::vec2(0.8f, 0.8f));
	addHeirarchicalGrid(glm::vec2(-3.2f, -1.72f), glm::vec2(0.8f, 0.8f));
	addHeirarchicalGrid(glm::vec2(3.2f, 1.35f), glm::vec2(0.8f, 0.8f));
	addHeirarchicalGrid(glm::vec2(3.2f, -1.72f), glm::vec2(0.8f, 0.8f));

	addHeirarchicalGrid(glm::vec2(0.3f, -27.1f), glm::vec2(1.1f, 0.9f));
	addHeirarchicalGrid(glm::vec2(1.3f, -30.74f), glm::vec2(2.6f, 1.0f));
	addHeirarchicalGrid(glm::vec2(0.026f, -52.09f), glm::vec2(1.1f, 0.9f));
}

void CollisionManager::addHeirarchicalGrid(const glm::vec2& pos, const glm::vec2& scale) {
	std::string name = "level";
	grids.at(name)->push_back(new HeirarchicalGrid(pos, scale));
	HeirarchicalGrid * grid = grids.at(name)->at(grids.at(name)->size() - 1);
	grid->left = pos.x - scale.x;
	grid->right = pos.x + scale.x;
	grid->top = pos.y - scale.y;
	grid->bottom = pos.y + scale.y;
}

bool CollisionManager::twoDcollisionCheck(const glm::vec2& position_A, const glm::vec2& scale_A, const float& left, const float& right, const float& top, const float& bottom) {
	bool collision = false;

	if ((position_A.x + (scale_A.x)) >= (left) // if right side on the right of left side
		&& position_A.x - (scale_A.x) <= (right)) // and left side is left of right side
	{
		if ((position_A.y + (scale_A.y)) >= (top)
			&& position_A.y - (scale_A.y) <= (bottom)) {
			collision = true;
		}
	}
	return collision;
}

unsigned int CollisionManager::getRelevantGrid(const glm::vec3& pos, const glm::vec3& scale) {
	unsigned int indexOfHeirGrid = 0;
	std::string name = "level";

	std::vector<HeirarchicalGrid*>* &grids = this->grids.at(name);
	for (unsigned int i = 0; i < grids->size(); i++) { //WORLD GRIDS
		if (twoDcollisionCheck(glm::vec2(pos.x, pos.z), glm::vec2(scale.x, scale.z), grids->at(i)->left, grids->at(i)->right, grids->at(i)->top, grids->at(i)->bottom)) {
			indexOfHeirGrid = i + 1;
			return indexOfHeirGrid;
		}
	}

	return 0; //for safeguard....shouldnt get here though
}

void CollisionManager::renderBoundingBoxes(Shader* shader, Renderer* renderer, const bool& addMode, const bool& rigidBodyMode, const glm::mat4& edit_modelMatrix) {
	shader->uniform("colour", red);
	shader->uniform("instanced", false);

	if (rigidBodyMode) {
		for (unsigned int h = 0; h < rigidBodies.size(); h++) {
			renderBox(rigidBodies.at(h), shader, renderer);
		}
	}
	else {
		std::map<std::string, std::vector<HeirarchicalGrid*>*>::iterator iter = this->grids.begin();
		for (iter = this->grids.begin(); iter != this->grids.end(); iter++) {
			std::vector<HeirarchicalGrid*>* &grids = (*iter).second;
			for (unsigned int h = 0; h < grids->size(); h++) {
				glm::mat4 model;
				glm::vec3 pos(grids->at(h)->position.x, 0.0f, grids->at(h)->position.y);
				glm::vec3 scale(grids->at(h)->scale.x, 0.2f, grids->at(h)->scale.y);
				Common::createModelMatrix(model, pos, 0.0f, scale);
				shader->uniform("uniform_model", model);
				renderer->drawElements(cube);
			}
		}
	}

	if (addMode) {
		shader->uniform("uniform_model", edit_modelMatrix);
		renderer->drawElements(cube);
	}
}

float CollisionManager::getHeirarchicalGridScale() { return 1.0f; }

void CollisionManager::displayGridDetails() {
	std::map<std::string, std::vector<HeirarchicalGrid*>*>::iterator iter = this->grids.begin();
	for (iter = this->grids.begin(); iter != this->grids.end(); iter++) {
		std::vector<HeirarchicalGrid*>* &grids = (*iter).second;
		for (unsigned int h = 0; h < grids->size(); h++) {
			std::cout << "P: " << grids->at(h)->position.x << ", " << grids->at(h)->position.y << "; S: " << grids->at(h)->scale.x << ", " << grids->at(h)->scale.y << std::endl;
		}
	}
}

std::vector<btRigidBody*>::iterator CollisionManager::getRigidBegin() { return rigidBodies.begin(); }
std::vector<btRigidBody*>::iterator CollisionManager::getRigidEnd() { return rigidBodies.end(); }

bool CollisionManager::readBoundFile() {
	// reading a text file
	string line;
	ifstream myfile;
	myfile = ifstream("../Engenius/Levels/LevelBounding.dat");

	if (myfile.is_open())
	{
		int numberOfBodies;
		bool gotNumber = false;
		int bodyNo = 0;
		while (std::getline(myfile, line, '\n')) //delimiter is '\n' in this
		{
			std::string boundType;
			glm::vec3 position;
			glm::vec3 scale;
			float mass;

			string asString;
			int variables = 8;
			if (!gotNumber) {
				string numAsString;
				getline(myfile, numAsString, '#');
				numberOfBodies = stoi(numAsString);
				gotNumber = true;
			}
			else if (bodyNo != numberOfBodies) {
				for (int i = 0; i < variables; i++) {
					getline(myfile, asString, ',');
					switch (i) {
					case 0: boundType = asString;			break;
					case 1: position.x = stof(asString);	break;
					case 2: position.y = stof(asString);	break;
					case 3: position.z = stof(asString);	break;
					case 4: scale.x = stof(asString);		break;
					case 5: scale.y = stof(asString);		break;
					case 6: scale.z = stof(asString);		break;
					case 7: mass = stof(asString);	break;
					}
				}//for loop
				bodyNo++;
				addBox(scale.x, scale.y, scale.z, position.x, position.y, position.z, mass);

			}
		}//while loop
		return true;
	}
	else {
		cout << "\nMission failed. We'll get em next time. \n. Unable to open file";
		return false;
	}
}

bool CollisionManager::writeBoundFile() {
	// writing on a text file
	ofstream myfile("../Engenius/Levels/LevelBounding.dat");
	if (myfile.is_open())
	{
		myfile << "BoundType | Positions | Scale | Mass \t//numberOfBodies on first line\n";
		myfile << rigidBodies.size() << "#\n";

		for (unsigned int i = 0; i < rigidBodies.size(); i++) {
			std::string boundType;
			btVector3 position, scale;
			float mass;

			if (rigidBodies[i]->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE) {
				boundType = "box";
				btTransform t;
				t.setIdentity();
				rigidBodies[i]->getMotionState()->getWorldTransform(t);
				position = t.getOrigin();
				scale = ((btBoxShape*)rigidBodies[i]->getCollisionShape())->getHalfExtentsWithMargin();
				mass = rigidBodies[i]->getInvMass();
			}

			myfile << boundType << ",";
			myfile << position.x() << ",";
			myfile << position.y() << ",";
			myfile << position.z() << ",";
			myfile << scale.x() << ",";
			myfile << scale.y() << ",";
			myfile << scale.z() << ",";
			myfile << mass << ",";
			myfile << "\n";
		}
		cout << "Bounding Saved\n";

		myfile.close();
		return true;
	}
	else {
		cout << "Unable to open file";
		return false;
	}
}

std::vector<HeirarchicalGrid*>::iterator CollisionManager::getHeirBegin() { return this->grids.at("level")->begin(); }
std::vector<HeirarchicalGrid*>::iterator CollisionManager::getHeirEnd() { return this->grids.at("level")->end(); }

void CollisionManager::update() {
	btSettings.world->stepSimulation(static_cast<btScalar>(1 / 60.0));
}

btRigidBody* CollisionManager::addBox(const float& width, const float& height, const float& depth, const float& x, const float& y, const float& z, const float& mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btBoxShape* box = new btBoxShape(btVector3(width, height, depth));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		box->calculateLocalInertia(mass, inertia);
	btMotionState* motion = new btDefaultMotionState(t);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);

	btRigidBody* body = new btRigidBody(info);
	btSettings.world->addRigidBody(body, COL_DEFAULT, COLLIDE_ALL);
	rigidBodies.push_back(body);

	return body;
}

btRigidBody* CollisionManager::addBox(const float& width, const float& height, const float& depth, const float& x, const float& y, const float& z, const float& mass, collisiontype& group, collisiontype& mask)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btBoxShape* box = new btBoxShape(btVector3(width, height, depth));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		box->calculateLocalInertia(mass, inertia);
	btMotionState* motion = new btDefaultMotionState(t);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);

	btRigidBody* body = new btRigidBody(info);
	btSettings.world->addRigidBody(body, group, mask);
	rigidBodies.push_back(body);

	return body;
}

btRigidBody* CollisionManager::addCapsule(const float& rad, const float& height, const float& x, const float& y, const float& z, const float& mass) {

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btCapsuleShape* capsule = new btCapsuleShape(rad, height);
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		capsule->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, capsule, inertia);

	btRigidBody* body = new btRigidBody(info);
	btSettings.world->addRigidBody(body, COL_DEFAULT, COLLIDE_ALL);

	return body;
}

void CollisionManager::renderCapsule(btRigidBody* capsule, Model *modelData, Shader* shader, Renderer* renderer) {

	if (capsule->getCollisionShape()->getShapeType() != CAPSULE_SHAPE_PROXYTYPE) //cout << "Wrong collision shape ";	
		return;

	float r = ((btCapsuleShape*)capsule->getCollisionShape())->getRadius();
	float h = ((btCapsuleShape*)capsule->getCollisionShape())->getHalfHeight() * 4;

	btTransform t;
	capsule->getMotionState()->getWorldTransform(t);
	glm::mat4 model;
	t.getOpenGLMatrix(glm::value_ptr(model));
	model = glm::scale(model, glm::vec3(r, h, r));

	shader->uniform("uniform_model", model);
	renderer->drawElements(cube);
}

void CollisionManager::renderBox(btRigidBody* box, Shader* shader, Renderer* renderer) {

	if (box->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE) 			//cout << "Wrong collision shape";
		return;

	btVector3 extent = ((btBoxShape*)box->getCollisionShape())->getHalfExtentsWithMargin();
	btTransform t;
	box->getMotionState()->getWorldTransform(t);

	glm::mat4 model;
	t.getOpenGLMatrix(glm::value_ptr(model));
	model = glm::scale(model, glm::vec3(extent.x(), extent.y(), extent.z())); //DEFINITELY goes after

	shader->uniform("uniform_model", model);
	renderer->drawElements(cube);
}

void CollisionManager::addToWorld(btRigidBody* body) {
	btSettings.world->addRigidBody(body);
}

void CollisionManager::addToWorld(btRigidBody* body, collisiontype COLL_TYPE, const int& collidesWith) {
	btSettings.world->addRigidBody(body, COLL_TYPE, collidesWith);
}

void CollisionManager::addGhostToWorld(btPairCachingGhostObject* ghost, collisiontype COLL_TYPE, const int& collidesWith) {
	btSettings.world->addCollisionObject(ghost, COLL_TYPE, collidesWith); // ?


																		  // world->addCollisionObject(ghostobject, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	btSettings.world->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}


void CollisionManager::addGhostToWorld(btPairCachingGhostObject* ghost) {
	btSettings.world->addCollisionObject(ghost, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter); // ?
	btSettings.world->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}

btBroadphasePair* CollisionManager::findWorldPair(const btBroadphasePair &pair) {
	return btSettings.world->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
}

void CollisionManager::removeObject(btRigidBody* body) {
	btSettings.world->removeCollisionObject(body);
}
void CollisionManager::removeObject(btPairCachingGhostObject* ghost) {
	btSettings.world->removeCollisionObject(ghost);
}

void CollisionManager::attachPlayerCollider(Character* player, const glm::vec3& pos, const float& rad, const float& height, const float& mass) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btCapsuleShape* playerShape = new btCapsuleShape(rad, height);
	btVector3 inertia(0, 0, 0);

	if (mass != 0.0)
		playerShape->calculateLocalInertia(mass, inertia);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, playerShape, inertia);

	btRigidBody* playerBody = new btRigidBody(info);
	playerBody->setAngularFactor(0); // Doesn't fall sideways
	playerBody->setSleepingThresholds(0.0, 0.0);
	int playerCollidesWith = COL_DEFAULT | COL_TRIGGER;
	// body, group, mask
	addToWorld(playerBody, COL_PLAYER, playerCollidesWith);
	playerBody->setActivationState(DISABLE_DEACTIVATION);
	playerBody->setFriction(static_cast<btScalar>(0.0f));

	// Now ghost
	btPairCachingGhostObject* playerGhost = new btPairCachingGhostObject();		// create object
	playerGhost->setCollisionShape(playerShape);								// set shape
	playerGhost->setWorldTransform(t);											// set world transform	
	playerGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);  // disable collision response // could also add CF_CHARACTER_OBJECT // If I knew what that flag did...

	addGhostToWorld(playerGhost, COL_PLAYER, playerCollidesWith);
	player->addCollider(playerBody, playerGhost);
}