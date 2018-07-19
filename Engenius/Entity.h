#ifndef ENTITY
#define ENTITY

#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Common.h"
#include <map>
#include "RenderProperties.h"

class Entity {
public:
	Entity(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const unsigned int& gridNo);

	Model* getModel();
	const glm::vec3 getPos();
	const glm::vec3 getScale();
	const float getYaw();
	//const glm::vec3 getInitialPos();
	//const glm::vec3 getInitialScale();
	//const float getInitialYaw();
	const glm::mat4 getModelMatrix();
	const unsigned int getGridNo();

	void setModelMatrix(const glm::mat4& newVal);
	void setPos(const glm::vec3& newVal);
	void setScale(const glm::vec3& newVal);
	void setYaw(const float& newVal);
	float getPitch();
	float getRoll();

	void setCullingBound(const float& newVal);
	float getCullingBound();

	void setPointLightIDs(const int& id1, const int& id2, const int& id3);
	int getPointLightID(const unsigned int& i);

	void setSpotLightIDs(const int& id1, const int& id2, const int& id3);
	int getSpotLightID(const unsigned int& i);

	int getZone();
	void setZone(const int& newVal);

	void setTiling(const float& newValue);
	float getTiling();

	RenderProperties_Uniforms* getRenderProperties();

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

	RenderProperties_Uniforms* renderProperties;

private:
	int pointLightIDs[3]; //lights that effect object
	int spotLightIDs[3]; //lights that effect object
	int zone;
	float tiling;
};
#endif