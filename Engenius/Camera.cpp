#include "Camera.h"

Camera::Camera(const int& screenWidth, const int& screenHeight) {
	ratio = float(screenWidth) / float(screenHeight); //make sure of this!!
	cameraEye = glm::vec3(0.0f, 0.75f, 0.0f);
	cameraAt = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
	fov = float(45.0f*DEG_TO_RADIAN);
	initialFOV = fov;
	pitch = 0.0f;
	yaw = 0.0f;
	projection = glm::perspective(fov, ratio, nearPlane, farPlane);
	view = glm::mat4(1.0);
	zoomedIn = false;
}

void Camera::updateScreenDimensions(const int& screenWidth, const int& screenHeight) {
	ratio = float(screenWidth) / float(screenHeight);
}

void Camera::Update(const glm::vec3& playerPosition, const int& viewMode) {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) *cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) *cos(glm::radians(pitch));
	cameraAt = glm::normalize(front);


	if (viewMode == 0) {
		cameraEye = playerPosition;
		cameraEye = glm::vec3(cameraEye.x, cameraEye.y + 1.5f, cameraEye.z);
		/*cameraEye = glm::vec3(cameraEye.x, cameraEye.y + 5.0, cameraEye.z);
		cameraEye = Common::moveForwardOrBack(cameraEye, yaw, -1.5f);
		cameraEye = Common::moveLateral(cameraEye, yaw, -4.0f);*/
	}
	else if (viewMode == 1) {
		cameraEye = playerPosition;
		cameraEye = glm::vec3(cameraEye.x, cameraEye.y + 20.0f, cameraEye.z);
		cameraEye = Common::moveLateral(cameraEye, yaw, -30.0f);
	}

	cameraRight = glm::normalize(glm::cross(cameraAt, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraAt));
	view = glm::lookAt(cameraEye, cameraEye + cameraAt, cameraUp);
}

void Camera::MoveForward(const float& dt_secs) {
	cameraEye += cameraAt * 150.0f * dt_secs;
}
void Camera::MoveBackward(const float& dt_secs) {
	cameraEye -= cameraAt * 150.0f * dt_secs;
}
void Camera::MoveLeft(const float& dt_secs) {
	cameraEye -= glm::normalize(glm::cross(cameraAt, cameraUp)) * 150.0f * dt_secs;
}
void Camera::MoveRight(const float& dt_secs) {
	cameraEye += glm::normalize(glm::cross(cameraAt, cameraUp)) * 150.0f * dt_secs;
}
void Camera::ToggleZoom() {
	zoomedIn = !zoomedIn;
	if (zoomedIn) {
		const float zoom = initialFOV / 2;
		if (fov > zoom)
			fov -= zoom;
		else
			fov = zoom;
		projection = glm::perspective(fov, ratio, nearPlane, farPlane);
	}
	else {
		if (fov < initialFOV)
			fov += initialFOV / 2;
		else
			fov = initialFOV;
		projection = glm::perspective(fov, ratio, nearPlane, farPlane);
	}
}

void Camera::passViewProjToShader(const unsigned int& shader) {
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::changeCameraPos(const glm::vec3& newVal) {
	cameraEye = newVal;
}

void Camera::setPitch(const float& newVal) {
	pitch = newVal;
}
void Camera::increasePitch(const float& newVal) {
	pitch += newVal;
}
void Camera::decreasePitch(const float& newVal) {
	pitch -= newVal;
}

void Camera::setYaw(const float& newVal) {
	yaw = newVal;
}
void Camera::increaseYaw(const float& newVal) {
	yaw += newVal;
}
void Camera::decreaseYaw(const float& newVal) {
	yaw -= newVal;
}


float Camera::getPitch() const {
	return pitch;
}
float Camera::getYaw() const {
	return yaw;
}

glm::mat4* Camera::getProjection_Pointer() {
	return &projection;
}
glm::mat4* Camera::getView_Pointer() {
	return &view;
}

glm::vec3* Camera::getCameraEye_Pointer() {
	return &cameraEye;
}

glm::mat4 Camera::getProjection() {
	return projection;
}
glm::mat4 Camera::getView() {
	return view;
}

glm::vec3 Camera::getCameraEye() {
	return cameraEye;
}

glm::vec3 Camera::getCameraAt() {
	return cameraAt;
}
glm::vec3 Camera::getCameraUp() {
	return cameraUp;
}
glm::vec3 Camera::getCameraDirection() {
	return cameraEye + cameraAt;
}

float Camera::getFOV() const {
	return fov;
}
float Camera::getRatio() const {
	return ratio;
}
float Camera::getNearPlane() const {
	return nearPlane;
}
const float* Camera::getFarPlane_Pointer() {
	return &farPlane;
}

const float Camera::getFarPlane() {
	return farPlane;
}