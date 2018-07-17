#ifndef LIGHTINGMANAGER
#define LIGHTINGMANAGER

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include "Uniforms.h"
#include "RenderProperties.h"

class LightingManager {
public:
	LightingManager(const glm::vec3& cameraEye, const glm::vec3& cameraAt);
	unsigned int getNumOfLights();
	void lightIDsToShader(const unsigned int& shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3);
	void lightsToShader(const unsigned int& shader);

	void lights_preloadShader(Uniforms* uni);
	void getLightIDs(Uniforms* uni, const int* po_1, const int* po_2, const int* po_3, const int* sp_1, const int* sp_2, const int* sp_3);
	void getShadowMapIDs(RenderProperties* rp, const int* po_1, const int* po_2, const int* po_3);

	std::tuple<int, int, int> getClosestLights(const glm::vec3& pos);
	std::tuple<int, int, int> getClosestSpotLights(const glm::vec3& pos);
	void setUpShadowRender_Pointlights(const unsigned int& shader, const int& lightIndex);
	void shadowMapsToShader(const unsigned int& shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3);
	void noShadowMessage(const unsigned int& shader);

	unsigned int getDepthCubeMap(const unsigned int& i);
	unsigned int getNumOfPointLights();
	glm::vec3 getPointLightPosition(const unsigned int& i);
	void changeExposure(const bool& increase);
	void toggleBloom();
	bool getIfBloom();
	void toggleShadow();
	bool getIfShadow();
	bool* getIfShadow_Pointer();
	float getExposure();
	void setExposure(const float& newVal);

	void moveLightForward(const float& dt_secs, const glm::vec3& cameraAt);
	void moveLightBackward(const float& dt_secs, const glm::vec3& cameraAt);
	void moveLightLeft(const float& dt_secs, const glm::vec3& cameraAt, const glm::vec3& cameraUp);
	void moveLightRight(const float& dt_secs, const glm::vec3& cameraAt, const glm::vec3& cameraUp);
	void moveLightUp(const float& dt_secs);
	void moveLightDown(const float& dt_secs);

	void setConstant(const bool& increase);
	void setLinear(const bool& increase);
	void setQuadratic(const bool& increase);

	void setAmbient(const bool& increase, const char& comp);
	void setDiffuse(const bool& increase, const char& comp);
	void setSpecular(const bool& increase, const char& comp);
	void setRange(const bool& increase);

	void displayLightDetails();

	void setPostProcessNum(const int& num);
	int getPostProcessNum();

private:
	void initLights(const glm::vec3& cameraEye, const glm::vec3& cameraAt);
	void initSpotLights(const glm::vec3& cameraEye, const glm::vec3& cameraAt);
	void initPointLights();
	void initShadows();

	int postProcessNum;

	bool editMode;

	struct DirectionalLight {
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		unsigned int depthMapFBO;
		unsigned int depthMap;
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
		unsigned int depthMapFBO;
		unsigned int depthCubeMap;
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

		unsigned int depthMapFBO;
		unsigned int depthMap;
	};

	DirectionalLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	
	const float valueChangeRate = 0.05f;

	const int defaultLightID = -1;
	const bool defaultFalse = false;
	const bool defaultTrue = true;

	unsigned int numOfLights = 0;
	const int MAX_LIGHTS = 3; //max lights to affect an object

	void addSpotLight(const glm::vec3& position, const glm::vec3& direction, const float& cutOff, const float& outerCutOff, const float& att_constant, const float& att_linear, const float& att_quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	void addPointLight(const glm::vec3& position, const float& att_constant, const float& att_linear, const float& att_quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	
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