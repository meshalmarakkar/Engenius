#ifndef CAMERA
#define CAMERA

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"
#include <glm/gtc/matrix_transform.hpp>
#define DEG_TO_RADIAN 0.017453293f

class Camera {
public:
	Camera(const int& screenWidth, const int& screenHeight);

	void Update(const glm::vec3& playerPos, const int& viewMode);
	void passViewProjToShader(const unsigned int& shader);

	void updateScreenDimensions(const int& screenWidth, const int& screenHeight);

	void MoveForward(const float& dt_secs);
	void MoveBackward(const float& dt_secs);
	void MoveLeft(const float& dt_secs);
	void MoveRight(const float& dt_secs);
	void ToggleZoom();

	void changeCameraPos(const glm::vec3& newVal);

	void setPitch(const float& newVal);
	void increasePitch(const float& newVal);
	void decreasePitch(const float& newVal);
	void setYaw(const float& newVal);
	void increaseYaw(const float& newVal);
	void decreaseYaw(const float& newVal);

	glm::mat4* getProjection_Pointer();
	glm::mat4* getView_Pointer();
	glm::vec3* getCameraEye_Pointer();
	glm::mat4 getProjection();
	glm::mat4 getView();
	glm::vec3 getCameraEye();
	glm::vec3 getCameraAt();
	glm::vec3 getCameraUp();
	glm::vec3 getCameraDirection();
	float getPitch() const;
	float getYaw() const;
	float getFOV() const;
	float getRatio() const;
	float getNearPlane() const;
	const float* getFarPlane_Pointer();
	const float getFarPlane();

private:
	float pitch;
	float yaw;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 cameraEye;
	glm::vec3 cameraAt;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	float initialFOV;
	float fov;
	float ratio;
	const float nearPlane = 0.1f;
	const float farPlane = 100.0f;
	bool zoomedIn;
};

#endif