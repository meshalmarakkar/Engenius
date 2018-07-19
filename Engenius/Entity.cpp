#include "Entity.h"

Entity::Entity(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const unsigned int& gridNo) : model(model), position(position), scale(scale), rotation(rotation), gridNo(gridNo) /*initialPosition(position), initialScale(scale), initialYaw(rotation.y)*/ {
	Common::createModelMatrix(this->modelMatrix, this->position, this->scale, this->rotation);
	tiling = 1.0f;

	renderProperties = new RenderProperties_Uniforms(&modelMatrix);
	renderProperties->getUniforms()->addUniform("tiling", &tiling);
	renderProperties->getUniforms()->addUniform("pointLightIDs[0]", &pointLightIDs[0]);
	renderProperties->getUniforms()->addUniform("pointLightIDs[1]", &pointLightIDs[1]);
	renderProperties->getUniforms()->addUniform("pointLightIDs[2]", &pointLightIDs[2]);
	renderProperties->getUniforms()->addUniform("spotLightIDs[0]", &spotLightIDs[0]);
	renderProperties->getUniforms()->addUniform("spotLightIDs[1]", &spotLightIDs[1]);
	renderProperties->getUniforms()->addUniform("spotLightIDs[2]", &spotLightIDs[2]);
}

const glm::vec3 Entity::getPos() { return this->position; }
const glm::vec3 Entity::getScale() { return this->scale; }
const float Entity::getYaw() { return this->rotation.y; }
//const glm::vec3 Entity::getInitialPos() { return this->initialPosition; }
//const glm::vec3 Entity::getInitialScale() { return this->initialScale; }
//const float Entity::getInitialYaw() { return this->initialYaw; }
const glm::mat4 Entity::getModelMatrix() { return this->modelMatrix; }
const unsigned int Entity::getGridNo() { return this->gridNo; }

void Entity::setModelMatrix(const glm::mat4& newVal) { this->modelMatrix = newVal; }
void Entity::setPos(const glm::vec3& newVal) { this->position = newVal; }
void Entity::setScale(const glm::vec3& newVal) { this->scale = newVal; }
void Entity::setYaw(const float& newVal) { this->rotation.y = newVal; }

float Entity::getPitch() {
	return rotation.x;
}

float Entity::getRoll() {
	return rotation.z;
}

int Entity::getZone() {
	return zone;
}

void Entity::setTiling(const float& newValue) {
	tiling = newValue;
}

float Entity::getTiling() {
	return tiling;
}

void Entity::setZone(const int& newVal) {
	zone = newVal;
}

void Entity::setCullingBound(const float& newVal) {
	cullingBound = newVal;
}
float Entity::getCullingBound() {
	return cullingBound;
}

Model* Entity::getModel() {
	return model;
}

void Entity::setPointLightIDs(const int& id1, const int& id2, const int& id3) {
	pointLightIDs[0] = id1;
	pointLightIDs[1] = id2;
	pointLightIDs[2] = id3;
}

int Entity::getPointLightID(const unsigned int& i) {
	return pointLightIDs[i];
}

void Entity::setSpotLightIDs(const int& id1, const int& id2, const int& id3) {
	spotLightIDs[0] = id1;
	spotLightIDs[1] = id2;
	spotLightIDs[2] = id3;
}

int Entity::getSpotLightID(const unsigned int& i) {
	return spotLightIDs[i];
}

RenderProperties_Uniforms* Entity::getRenderProperties() {
	return renderProperties;
}