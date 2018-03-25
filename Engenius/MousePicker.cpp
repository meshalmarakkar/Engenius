#include "MousePicker.h"

MousePicker::MousePicker(WindowManager* windowManager, EntityManager* entityManager, TerrainManager* terrainManager) : windowManager(windowManager), entityManager(entityManager), terrainManager(terrainManager){ ; }

glm::vec2 MousePicker::getNormalizedDeviceCoords(const float mouseX, const float mouseY) {
	float x = (2.0f * mouseX) / windowManager->getScreenWidth() - 1.0f;
	float y = (2.0f * mouseY) / windowManager->getScreenHeight() - 1.0f;
	return glm::vec2(x, -y); //coordinate system from top-left
}

glm::vec4 MousePicker::getEyeCoords(const glm::vec4 clipCoords, const glm::mat4 projection) {
	glm::mat4 inverseProjection = glm::inverse(projection);
	glm::vec4 eyeCoords = inverseProjection * clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::getWorldCoords(const glm::vec4 eyeCoords, const glm::mat4 view) {
	glm::mat4 inverseView = glm::inverse(view);
	glm::vec4 ray_worldSpace = inverseView * eyeCoords;
	glm::vec3 mouseRay(ray_worldSpace.x, ray_worldSpace.y, ray_worldSpace.z);
	mouseRay = glm::normalize(mouseRay);
	return mouseRay;
}

glm::vec3 MousePicker::calculateMouseRay(const float mouseX, const float mouseY, const glm::mat4 view, const glm::mat4 projection) {
	glm::vec2 normDeviceCoords(getNormalizedDeviceCoords(mouseX, mouseY));
	glm::vec4 clipCoords(normDeviceCoords.x, normDeviceCoords.y, -1.0f, 1.0f); //z points at negative 1 as ur lookin into screen
	glm::vec4 eyeCoords(getEyeCoords(clipCoords, projection));
	glm::vec3 worldRay(getWorldCoords(eyeCoords, view));
	return worldRay;
}

glm::vec3 MousePicker::getPointOnRay(const glm::vec3 ray, const float distance) {
	glm::vec3 scaledRay(ray.x * distance, ray.y * distance, ray.z * distance);
	return glm::vec3(cameraEye + scaledRay);
}

bool MousePicker::isUnderGround(const glm::vec3 testPoint) {
	if (testPoint.y < terrainManager->getTerrainHeight(testPoint.x, testPoint.z))
		return true;
	else
		return false;
}

bool MousePicker::intersectionInRange(const float start, const float finish, const glm::vec3 ray) {
	glm::vec3 startPoint = getPointOnRay(ray, start);
	glm::vec3 endPoint = getPointOnRay(ray, finish);
	if (!isUnderGround(startPoint) && isUnderGround(endPoint)) {
		return true;
	}
	else {
		return false;
	}
}

glm::vec3 MousePicker::binarySearch(const int count, const float start, const float finish, const glm::vec3 ray) {
	const float half = start + ((finish - start) / 2.0f);
	if (count >= 200) {
		glm::vec3 endPoint = getPointOnRay(ray, half);
		const float Y = terrainManager->getTerrainHeight(endPoint.x, endPoint.z);
		return glm::vec3(endPoint.x, Y, endPoint.z);
	}
	if (intersectionInRange(start, half, ray)) {
		return binarySearch(count + 1, start, half, ray);
	}
	else {
		return binarySearch(count + 1, half, finish, ray);
	}
}

glm::vec3 MousePicker::TerrainPointOfMouse(const glm::vec3 cameraEye, const glm::mat4 view, const glm::mat4 projection) {
	this->cameraEye = cameraEye;
	currentRay = calculateMouseRay(static_cast<float>(windowManager->getScreenWidth() / 2), static_cast<float>(windowManager->getScreenHeight() / 2), view, projection);
	const float RANGE = 600.0f;
	glm::vec3 currentTerrainPoint;
	if (intersectionInRange(0, RANGE, currentRay)) {
		currentTerrainPoint = binarySearch(0, 0, RANGE, currentRay);
	}
	else {
		currentTerrainPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	return currentTerrainPoint;
}

//unsigned int MousePicker::SelectObject(const glm::vec3 cameraEye, const glm::mat4 view, const glm::mat4 projection) {
//	this->cameraEye = cameraEye;
//	const std::vector<unsigned int> &renderedObjects = entityManager->getIndicesOfRenderedObjects();
//	currentRay = calculateMouseRay(static_cast<float>(MidX), static_cast<float>(MidY), view, projection);
//	const float RANGE = 600.0f;
//
//	glm::vec3 orig = getPointOnRay(currentRay, 0);
//	glm::vec3 dir = getPointOnRay(currentRay, 600);
//
// 	for (unsigned int i = 0; i < renderedObjects.size(); i++) {
//		const glm::vec3 min = entityManager->getObject(renderedObjects.at(i))->getBoundingBox()->getMin();
//		const glm::vec3 max = entityManager->getObject(renderedObjects.at(i))->getBoundingBox()->getMax();
//
//		float tmin = (min.x - orig.x) / dir.x;
//		float tmax = (max.x - orig.x) / dir.x;
//
//		if (tmin > tmax) swap(tmin, tmax);
//
//		float tymin = (min.y - orig.y) / dir.y;
//		float tymax = (max.y - orig.y) / dir.y;
//
//		if (tymin > tymax) swap(tymin, tymax);
//
//		if ((tmin > tymax) || (tymin > tmax))
//			return false;
//
//		if (tymin > tmin)
//			tmin = tymin;
//
//		if (tymax < tmax)
//			tmax = tymax;
//
//		float tzmin = (min.z - orig.z) / dir.z;
//		float tzmax = (max.z - orig.z) / dir.z;
//
//		if (tzmin > tzmax) swap(tzmin, tzmax);
//
//		if (!(tmin > tzmax) || !(tzmin > tmax)) {
//			//return 0;
//
//			if (tzmin > tmin)
//				tmin = tzmin;
//
//			if (tzmax < tmax)
//				tmax = tzmax;
//
//			return renderedObjects.at(i);
//		}
//	}
//	return 0;
//}