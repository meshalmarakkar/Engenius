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
#include "Shader.h"

class LightingManager {
public:
	LightingManager(const glm::vec3& cameraEye, const glm::vec3& cameraAt);
	unsigned int getNumOfLights();
	void lightIDsToShader(const unsigned int& shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3);

	void lights_preloadShader(Uniforms* uni);
	void getLightIDs(Uniforms* uni, const int* po_1, const int* po_2, const int* po_3, const int* sp_1, const int* sp_2, const int* sp_3);
	void getShadowMapIDs(RenderProperties* rp, const int* po_1, const int* po_2, const int* po_3);

	std::tuple<int, int, int> getClosestLights(const glm::vec3& pos);
	std::tuple<int, int, int> getClosestSpotLights(const glm::vec3& pos);
	void setUpShadowRender_Pointlights(const unsigned int& shader, const int& lightIndex);
	void shadowMapsToShader(const unsigned int& shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3);
	void noShadowMessage(const unsigned int& shader);

	void shadowMapsToShader(Shader* shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3);
	void noShadowMessage(Shader* shader);

	unsigned int getDepthCubeMap(const unsigned int& i);
	unsigned int getNumOfPointLights();
	glm::vec3 getPointLightPosition(const unsigned int& i);
	void changeExposure(const bool& increase);
	void toggleBloom();
	bool getIfBloom();
	void toggleShadow();
	bool getIfShadow();
	float getExposure();
	void setExposure(const float& newVal);

	const float* getShadowFarPlane();

	void setPostProcessNum(const int& num);
	int getPostProcessNum();

private:
	void initLights(const glm::vec3& cameraEye, const glm::vec3& cameraAt);
	void initSpotLights(const glm::vec3& cameraEye, const glm::vec3& cameraAt);
	void initPointLights();
	void initShadows();

	void addSpotLight(const glm::vec3& position, const glm::vec3& direction, const float& cutOff, const float& outerCutOff, const float& att_constant, const float& att_linear, const float& att_quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	void addPointLight(const glm::vec3& position, const float& att_constant, const float& att_linear, const float& att_quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

private:
	int postProcessNum;

	bool editMode;
	
	#define SHADOW_TRANSFORMS 6
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
		glm::mat4 shadowTransforms[SHADOW_TRANSFORMS];
		
		PointLight(const glm::vec3& pos, const float& con, const float& lin, const float& qua, const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const unsigned int& id, const float& range, const glm::mat4 shadowTrans[SHADOW_TRANSFORMS])
		: position(pos), att_constant(con), att_linear(lin), att_quadratic(qua), ambient(amb), diffuse(dif), specular(spe), id(id), range(range){
			for (int i = 0; i < SHADOW_TRANSFORMS; i++) {
				shadowTransforms[i] = shadowTrans[i];
			}
		}
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

	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	
	const float valueChangeRate = 0.05f;

	const int defaultLightID = -1;
	const bool defaultFalse = false;
	const bool defaultTrue = true;

	unsigned int numOfLights = 0;
	const int MAX_LIGHTS = 3; //max lights to affect an object

	bool bloom;
	float exposure;

	////SHADOWS////
	bool shadow;

	const float SHADOW_WIDTH = 1024;
	const float SHADOW_ASPECT = SHADOW_WIDTH / SHADOW_WIDTH;
	const float SHADOW_NEAR_PLANE = 1.0f;
	const float SHADOW_FAR_PLANE = 25.0f;
};

#endif