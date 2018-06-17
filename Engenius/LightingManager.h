#ifndef LIGHTINGMANAGER
#define LIGHTINGMANAGER

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>

class LightingManager {
public:
	LightingManager(glm::vec3 cameraEye, glm::vec3 cameraAt);
	unsigned int getNumOfLights();
	void lightIDsToShader(GLuint shader, int po_1, int po_2, int po_3, int sp_1, int sp_2, int sp_3);
	void lightsToShader(GLuint shader);
	std::tuple<int, int, int> getClosestLights(glm::vec3 pos);
	std::tuple<int, int, int> getClosestSpotLights(glm::vec3 pos);
	void setUpShadowRender_Pointlights(GLuint shader, int lightIndex);
	void shadowMapsToShader(GLuint shader, int po_1, int po_2, int po_3, int sp_1, int sp_2, int sp_3);
	void noShadowMessage(GLuint shader);

	GLuint getDepthCubeMap(unsigned int i);
	unsigned int getNumOfPointLights();
	glm::vec3 getPointLightPosition(unsigned int i);
	void changeExposure(bool increase);
	void toggleBloom();
	bool getIfBloom();
	void toggleShadow();
	bool getIfShadow();
	float getExposure();
	void setExposure(float newVal);

	void moveLightForward(const float dt_secs, const glm::vec3 cameraAt);
	void moveLightBackward(const float dt_secs, const glm::vec3 cameraAt);
	void moveLightLeft(const float dt_secs, const glm::vec3 cameraAt, const glm::vec3 cameraUp);
	void moveLightRight(const float dt_secs, const glm::vec3 cameraAt, const glm::vec3 cameraUp);
	void moveLightUp(const float dt_secs);
	void moveLightDown(const float dt_secs);

	void setConstant(bool increase);
	void setLinear(bool increase);
	void setQuadratic(bool increase);

	void setAmbient(bool increase, char comp);
	void setDiffuse(bool increase, char comp);
	void setSpecular(bool increase, char comp);
	void setRange(bool increase);

	void displayLightDetails();

	void setPostProcessNum(int num);
	int getPostProcessNum();

private:
	void initLights(glm::vec3 cameraEye, glm::vec3 cameraAt);
	void initSpotLights(glm::vec3 cameraEye, glm::vec3 cameraAt);
	void initPointLights();
	void initShadows();

	int postProcessNum;

	bool editMode;

	struct DirectionalLight {
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		GLuint depthMapFBO;
		GLuint depthMap;
	};

	struct PointLight {
		glm::vec3 position;

		float att_constant;
		float att_linear;
		float att_quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		unsigned int id;
		float range;

		std::vector<unsigned int> id_objectsInRange;
		GLuint depthMapFBO;
		GLuint depthCubeMap;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;

		float att_constant;
		float att_linear;
		float att_quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		unsigned int id;
		float range;

		std::vector<unsigned int> id_objectsInRange;

		GLuint depthMapFBO;
		GLuint depthMap;
	};

	DirectionalLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	
	const float valueChangeRate = 0.05f;

	unsigned int numOfLights = 0;
	const int MAX_LIGHTS = 3; //max lights to affect an object

	void addSpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float att_constant, float att_linear, float att_quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void addPointLight(glm::vec3 position, float att_constant, float att_linear, float att_quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	
	bool bloom;
	float exposure;

	////SHADOWS////
	bool shadow;

	const GLfloat SHADOW_WIDTH = 1024;
	const GLfloat SHADOW_ASPECT = SHADOW_WIDTH / SHADOW_WIDTH;
	const GLfloat SHADOW_NEAR_PLANE = 1.0f;
	const GLfloat SHADOW_FAR_PLANE = 25.0f;
};

#endif