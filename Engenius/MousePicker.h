#ifndef MOUSEPICKER
#define MOUSEPICKER

#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "WindowManager.h"
#include "TerrainManager.h"

class MousePicker {
public:
	MousePicker(WindowManager* windowManager, EntityManager* entityManager, TerrainManager* terrainManager);
	glm::vec3 TerrainPointOfMouse(const glm::vec3 cameraEye, const glm::mat4 view, const glm::mat4 projection);
	//	unsigned int SelectObject(const glm::vec3 cameraEye, const glm::mat4 view, const glm::mat4 projection);

private:
	bool isUnderGround(const glm::vec3 testPoint);
	bool intersectionInRange(const float start, const float finish, const glm::vec3 ray);
	glm::vec3 getPointOnRay(const glm::vec3 ray, const float distance);
	glm::vec3 binarySearch(const int count, const float start, const float finish, const glm::vec3 ray);

	glm::vec2 getNormalizedDeviceCoords(const float mouseX, const float mouseY);
	glm::vec4 getEyeCoords(const glm::vec4 clipCoords, const glm::mat4 projection);
	glm::vec3 getWorldCoords(const glm::vec4 eyeCoords, const glm::mat4 view);
	glm::vec3 calculateMouseRay(const float mouseX, const float mouseY, const glm::mat4 view, const glm::mat4 projection);

	glm::vec3 cameraEye;
	EntityManager* entityManager;
	WindowManager* windowManager;
	TerrainManager* terrainManager;
	glm::vec3 currentRay;
};

#endif