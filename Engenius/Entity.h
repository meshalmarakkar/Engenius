#ifndef ENTITY
#define ENTITY

#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Common.h"

class Entity {
public:
	Entity(Model* model, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, unsigned int gridNo);

	Model* getModel();
	const glm::vec3 getPos();
	const glm::vec3 getScale();
	const float getYaw();
	//const glm::vec3 getInitialPos();
	//const glm::vec3 getInitialScale();
	//const float getInitialYaw();
	const glm::mat4 getModelMatrix();
	const unsigned int getGridNo();

	void setModelMatrix(const glm::mat4 newVal);
	void setPos(const glm::vec3 newVal);
	void setScale(const glm::vec3 newVal);
	void setYaw(const float newVal);
	float getPitch();
	float getRoll();

	void setCullingBound(const float newVal);
	float getCullingBound();

	void setPointLightIDs(int id1, int id2);
	int getPointLightID(unsigned int i);

	void setSpotLightIDs(int id1, int id2);
	int getSpotLightID(unsigned int i);

	int getZone();
	void setZone(int newVal);

	void setShininess(const float newVal);
	void shininessToShader(GLuint shader);

	void setTiling(const float newValue);
	float getTiling();

protected:
	Model * model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	//glm::vec3 initialPosition;
	//glm::vec3 initialScale;
	//float initialYaw;
	glm::mat4 modelMatrix;
	float cullingBound;
	unsigned int gridNo;

private:
	int pointLightIDs[2]; //lights that effect object
	int spotLightIDs[2]; //lights that effect object
	float shininess;
	int zone;
	float tiling;
};
#endif