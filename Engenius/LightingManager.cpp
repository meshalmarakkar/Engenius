#include "LightingManager.h"

LightingManager::LightingManager(const glm::vec3& cameraEye, const glm::vec3& cameraAt) {
	initLights(cameraEye, cameraAt);
	initShadows();
	exposure = 5.0f;
	bloom = false;
	editMode = false;
	shadow = true;
	postProcessNum = 0;
}

void LightingManager::initShadows() {
	//for less precision...
	int internal_format = GL_DEPTH_COMPONENT24;
	GLenum data_type = GL_UNSIGNED_INT;

	for (unsigned int i = 0; i < pointLights.size(); i++) {
		// Create depth cubemap texture
		glGenTextures(1, &pointLights[i].depthCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[i].depthCubeMap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		for (unsigned int face = 0; face < 6; ++face)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT32, (GLsizei)SHADOW_WIDTH, (GLsizei)SHADOW_WIDTH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		// Attach cubemap as depth map FBO's color buffer
		glGenFramebuffers(1, &pointLights[i].depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, pointLights[i].depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointLights[i].depthCubeMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void LightingManager::initSpotLights(const glm::vec3& cameraEye, const glm::vec3& cameraAt) {
	//addSpotLight(glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, -1.0f), std::cos(glm::radians(42.5f)), std::cos(glm::radians(47.5f)), 1.0f, 0.09f, 0.032f, glm::vec3(0.5f, 0.1f, 0.1f), glm::vec3(0.5f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f));

}

void LightingManager::initPointLights() {
	/*const float att_const = 5.0f;
	const float att_lin = 0.5f;
	const float att_quad = 5.0f;
*/
	const float att_const = 0.5f;
	const float att_lin  = -2.5f;
	const float att_quad = 23.0f;

	const glm::vec3 ambient = glm::vec3(5.0f, 5.0f, 5.0f);
	const glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//const glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	//const glm::vec3 diffuse(0.1f, 0.1f, 0.1f);
	//const glm::vec3 specular(0.1f, 0.1f, 0.1f);

	addPointLight(glm::vec3(0.0f, 3.9f, -2.0f), att_const, att_lin, att_quad, ambient, diffuse, specular);
	addPointLight(glm::vec3(0.0f, 3.9f, -13.0f), att_const, att_lin, att_quad, ambient, diffuse, specular);
	addPointLight(glm::vec3(0.0f, 3.9f, -25.0f), att_const, att_lin, att_quad, ambient, diffuse, specular);
	addPointLight(glm::vec3(0.0f, 3.9f, -32.0f), att_const, att_lin, att_quad, ambient, diffuse, specular);
	addPointLight(glm::vec3(0.0f, 3.9f, -48.0f), att_const, att_lin, att_quad, ambient, diffuse, specular);
}

void LightingManager::initLights(const glm::vec3& cameraEye, const glm::vec3& cameraAt) {
	initPointLights();
	initSpotLights(cameraEye, cameraAt);	
}

void LightingManager::addSpotLight(const glm::vec3& position, const glm::vec3& direction, const float& cutOff, const float& outerCutOff, const float& att_constant, const float& att_linear, const float& att_quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
	SpotLight light = {
		position,
		direction,
		cutOff,
		outerCutOff,
		att_constant,
		att_linear,
		att_quadratic,
		ambient,
		diffuse,
		specular,
	};
	light.range = 200.0f;
	spotLights.push_back(light);
	spotLights.back().id = spotLights.size();
	numOfLights++;
}

void LightingManager::addPointLight(const glm::vec3& position, const float& att_constant, const float& att_linear, const float& att_quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
	PointLight light = {
		position,
		att_constant, att_linear, att_quadratic,
		ambient,
		diffuse,
		specular,
		0,
		25.0f
	};
	pointLights.push_back(light);
	pointLights.back().id = pointLights.size() - 1;
	numOfLights++;
}

void LightingManager::lightsToShader(const unsigned int& shader) {
	//UPDATE ARRAY SIZE IN SHADERS!!
	for (unsigned int lightNo = 0; lightNo < pointLights.size(); lightNo++) {
		std::string number = std::to_string(lightNo);
		unsigned int uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].position").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(pointLights.at(lightNo).position));
		uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].ambient").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(pointLights.at(lightNo).ambient));
		uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].diffuse").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(pointLights.at(lightNo).diffuse));
		uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].specular").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(pointLights.at(lightNo).specular));
		uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].constant").c_str());
		glUniform1f(uniformIndex, pointLights.at(lightNo).att_constant);
		uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].linear").c_str());
		glUniform1f(uniformIndex, pointLights.at(lightNo).att_linear);
		uniformIndex = glGetUniformLocation(shader, ("pointLights[" + number + "].quadratic").c_str());
		glUniform1f(uniformIndex, pointLights.at(lightNo).att_quadratic);
	}

	for (unsigned int lightNo = 0; lightNo < spotLights.size(); lightNo++) {
		std::string number = std::to_string(lightNo);
		unsigned int uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].position").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(spotLights.at(lightNo).position));
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].direction").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(spotLights.at(lightNo).direction));
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].cutOff").c_str());
		glUniform1f(uniformIndex, spotLights.at(lightNo).cutOff);
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].outerCutOff").c_str());
		glUniform1f(uniformIndex, spotLights.at(lightNo).outerCutOff);
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].constant").c_str());
		glUniform1f(uniformIndex, spotLights.at(lightNo).att_constant);
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].linear").c_str());
		glUniform1f(uniformIndex, spotLights.at(lightNo).att_linear);
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].quadratic").c_str());
		glUniform1f(uniformIndex, spotLights.at(lightNo).att_quadratic);
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].ambient").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(spotLights.at(lightNo).ambient));
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].diffuse").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(spotLights.at(lightNo).diffuse));
		uniformIndex = glGetUniformLocation(shader, ("spotLights[" + number + "].specular").c_str());
		glUniform3fv(uniformIndex, 1, glm::value_ptr(spotLights.at(lightNo).specular));
	}
}

void LightingManager::lights_preloadShader(Uniforms* uni) {
	for (unsigned int lightNo = 0; lightNo < pointLights.size(); lightNo++) {
		std::string number = std::to_string(lightNo);
		uni->addUniform(("pointLights[" + number + "].position"), &(pointLights.at(lightNo).position));
		uni->addUniform(("pointLights[" + number + "].ambient"), &(pointLights.at(lightNo).ambient));
		uni->addUniform(("pointLights[" + number + "].diffuse"), &(pointLights.at(lightNo).diffuse));
		uni->addUniform(("pointLights[" + number + "].specular"), &(pointLights.at(lightNo).specular));
		uni->addUniform(("pointLights[" + number + "].constant"), &(pointLights.at(lightNo).att_constant));
		uni->addUniform(("pointLights[" + number + "].linear"), &(pointLights.at(lightNo).att_linear));
		uni->addUniform(("pointLights[" + number + "].quadratic"), &(pointLights.at(lightNo).att_quadratic));
	}

	for (unsigned int lightNo = 0; lightNo < spotLights.size(); lightNo++) {
		std::string number = std::to_string(lightNo);
		uni->addUniform(("spotLights[" + number + "].position"), &(spotLights.at(lightNo).position));
		uni->addUniform(("spotLights[" + number + "].ambient"), &(spotLights.at(lightNo).ambient));
		uni->addUniform(("spotLights[" + number + "].diffuse"), &(spotLights.at(lightNo).diffuse));
		uni->addUniform(("spotLights[" + number + "].specular"), &(spotLights.at(lightNo).specular));
		uni->addUniform(("spotLights[" + number + "].cutOff"), &(spotLights.at(lightNo).cutOff));
		uni->addUniform(("spotLights[" + number + "].outerCutOff"), &(spotLights.at(lightNo).outerCutOff));
		uni->addUniform(("spotLights[" + number + "].constant"), &(spotLights.at(lightNo).att_constant));
		uni->addUniform(("spotLights[" + number + "].linear"), &(spotLights.at(lightNo).att_linear));
		uni->addUniform(("spotLights[" + number + "].quadratic"), &(spotLights.at(lightNo).att_quadratic));
	}
}

void LightingManager::getLightIDs(Uniforms* uni, const int* po_1, const int* po_2, const int* po_3, const int* sp_1, const int* sp_2, const int* sp_3) {
//	unsigned int uniformIndex;
	std::string number;
	for (int lightNo = 0; lightNo < MAX_LIGHTS; lightNo++) {
		const int* id = &defaultLightID; //only checks calc in shader if id greater than -1
		switch (lightNo) {
		case 0:
			id = po_1;
			break;
		case 1:
			id = po_2;
			break;
		case 2:
			id = po_3;
			break;
		}

		number = std::to_string(lightNo);
		uni->addUniform(("pointLightIDs[" + number + "]"), id);

	/*	uniformIndex = glGetUniformLocation(shader, ("pointLightIDs[" + number + "]").c_str());
		glUniform1i(uniformIndex, id);*/
		id = &defaultLightID;
		switch (lightNo) {
		case 0:
			id = sp_1;
			break;
		case 1:
			id = sp_2;
			break;
		case 2:
			id = sp_3;
			break;
		}
		number = std::to_string(lightNo);
		uni->addUniform(("spotLightIDs[" + number + "]"), id);
		/*uniformIndex = glGetUniformLocation(shader, ("spotLightIDs[" + number + "]").c_str());
		glUniform1i(uniformIndex, id);*/
	}
}

void LightingManager::lightIDsToShader(const unsigned int& shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3) {
	unsigned int uniformIndex;
	std::string number;
	for (int lightNo = 0; lightNo < MAX_LIGHTS; lightNo++) {
		int id = -1; //only checks calc in shader if id greater than -1
		switch (lightNo) {
			case 0:
				id = po_1;
				break;
			case 1:
				id = po_2;
				break;
			case 2:
				id = po_3;
				break;
		}

		number = std::to_string(lightNo);
		uniformIndex = glGetUniformLocation(shader, ("pointLightIDs[" + number + "]").c_str());
		glUniform1i(uniformIndex, id);

		if (lightNo == 0)
			id = sp_1; 
		else
			id = sp_2; 
		number = std::to_string(lightNo);
		uniformIndex = glGetUniformLocation(shader, ("spotLightIDs[" + number + "]").c_str());
		glUniform1i(uniformIndex, id);
	}
}

void LightingManager::setUpShadowRender_Pointlights(const unsigned int& shader, const int& lightIndex) {
	glBindFramebuffer(GL_FRAMEBUFFER, pointLights[lightIndex].depthMapFBO);
	glViewport(0, 0, (GLsizei)SHADOW_WIDTH, (GLsizei)SHADOW_WIDTH);
	glClear(GL_DEPTH_BUFFER_BIT); // clear FBO
	glUseProgram(shader);

	glm::vec3 lightPos = getPointLightPosition(lightIndex);
	glUniform1f(glGetUniformLocation(shader, "far_plane"), SHADOW_FAR_PLANE);
	glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), SHADOW_ASPECT, SHADOW_NEAR_PLANE, SHADOW_FAR_PLANE); //perspective projection is the best suited for this

	//TODO: PRE-MAKE THESE
	const int SHADOW_TRANSFORMS = 6;
	glm::mat4 shadowTransforms[SHADOW_TRANSFORMS];

	shadowTransforms[0] = shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	shadowTransforms[1] = shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	shadowTransforms[2] = shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	shadowTransforms[3] = shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	shadowTransforms[4] = shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	shadowTransforms[5] = shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

	for (int i = 0; i < SHADOW_TRANSFORMS; i++) {
		glUniformMatrix4fv(glGetUniformLocation(shader, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
	}
}

void LightingManager::getShadowMapIDs(RenderProperties* rp, const int* po_1, const int* po_2, const int* po_3) {
	// pass in the shadowmap 
	for (int iter = 0; iter < MAX_LIGHTS; iter++) {
		const int* i = &defaultLightID;// -1 as ids start at 0;
		switch (iter) {
		case 0:
			i = po_1;
			break;
		case 1:
			i = po_2;
			break;
		case 2:
			i = po_3;
			break;
		}

		std::string number = std::to_string(iter);
		if (*i > -1) {
			rp->getMaterial()->addTexture_Cubemap(TextureType(("depthMap[" + std::to_string(iter) + "]"), pointLights[*i].depthCubeMap));
			// pass in the shadowmaps, each in a different texture unit
			/*unsigned int uniformIndex = glGetUniformLocation(shader, ("depthMap[" + std::to_string(iter) + "]").c_str());
			glActiveTexture(GL_TEXTURE8 + iter);
			glUniform1i(uniformIndex, 8 + iter);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[*i].depthCubeMap);*/
			rp->getUniforms()->addUniform(("depthMap_ifRender[" + std::to_string(iter) + "]"), &defaultTrue);

			/*uniformIndex = glGetUniformLocation(shader, ("depthMap_ifRender[" + std::to_string(iter) + "]").c_str());
			glUniform1i(uniformIndex, true);*/
		}
		else {
			rp->getMaterial()->addTexture_Cubemap(TextureType(("depthMap[" + std::to_string(iter) + "]"), pointLights[0].depthCubeMap));

			// pass in the shadowmaps, each in a different texture unit
		/*	unsigned int uniformIndex = glGetUniformLocation(shader, ("depthMap[" + std::to_string(iter) + "]").c_str());
			glActiveTexture(GL_TEXTURE8 + iter);
			glUniform1i(uniformIndex, 8 + iter);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[0].depthCubeMap);*/
			rp->getUniforms()->addUniform(("depthMap_ifRender[" + std::to_string(iter) + "]"), &defaultFalse);

			//uniformIndex = glGetUniformLocation(shader, ("depthMap_ifRender[" + std::to_string(iter) + "]").c_str());
			//glUniform1i(uniformIndex, false);
		}
	}
}

void LightingManager::shadowMapsToShader(const unsigned int& shader, const int& po_1, const int& po_2, const int& po_3, const int& sp_1, const int& sp_2, const int& sp_3) {
	// pass in the shadowmap 
	for (int iter = 0; iter < MAX_LIGHTS; iter++) {
		int i = -1;// -1 as ids start at 0;
		switch (iter) {
			case 0:
				i = po_1;
				break;
			case 1:
				i = po_2;
				break;
			case 2:
				i = po_3;
				break;
		}

		if (i > -1) {
			std::string number = std::to_string(iter);
			// pass in the shadowmaps, each in a different texture unit
			unsigned int uniformIndex = glGetUniformLocation(shader, ("depthMap[" + std::to_string(iter) + "]").c_str());
			glActiveTexture(GL_TEXTURE8 + iter);
			glUniform1i(uniformIndex, 8 + iter);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[i].depthCubeMap);

			uniformIndex = glGetUniformLocation(shader, ("depthMap_ifRender[" + std::to_string(iter) + "]").c_str());
			glUniform1i(uniformIndex, true);
		}
		else {
			std::string number = std::to_string(iter);
			// pass in the shadowmaps, each in a different texture unit
			unsigned int uniformIndex = glGetUniformLocation(shader, ("depthMap[" + std::to_string(iter) + "]").c_str());
			glActiveTexture(GL_TEXTURE8 + iter);
			glUniform1i(uniformIndex, 8 + iter);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[0].depthCubeMap);

			uniformIndex = glGetUniformLocation(shader, ("depthMap_ifRender[" + std::to_string(iter) + "]").c_str());
			glUniform1i(uniformIndex, false);
		}
	}
}

void LightingManager::noShadowMessage(const unsigned int& shader) {
	for (int iter = 0; iter < MAX_LIGHTS; iter++) {
		std::string number = std::to_string(iter);
		// pass in the shadowmaps, each in a different texture unit
		unsigned int uniformIndex = glGetUniformLocation(shader, ("depthMap[" + std::to_string(iter) + "]").c_str());
		glActiveTexture(GL_TEXTURE8 + iter);
		glUniform1i(uniformIndex, 8 + iter);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[0].depthCubeMap);

		uniformIndex = glGetUniformLocation(shader, ("depthMap_ifRender[" + std::to_string(iter) + "]").c_str());
		glUniform1i(uniformIndex, false);
	}
}

unsigned int LightingManager::getDepthCubeMap(const unsigned int& i) {
	return pointLights[i].depthCubeMap;
}

glm::vec3 LightingManager::getPointLightPosition(const unsigned int& i) {
	return pointLights.at(i).position;
}

unsigned int LightingManager::getNumOfPointLights() {
	return pointLights.size();
}

unsigned int LightingManager::getNumOfLights() {
	return numOfLights;
}

void LightingManager::changeExposure(const bool& increase) {
	if (increase)
		exposure += 0.5f;
	else
		exposure -= 0.5f;
}

void LightingManager::setExposure(const float& newVal) {
	exposure = newVal;
}

void LightingManager::toggleBloom() {
	bloom = !bloom;
	std::cout << "bloom: " << (bloom ? "on" : "off") << std::endl;
}

bool LightingManager::getIfBloom() {
	return bloom;
}

float LightingManager::getExposure() {
	return exposure;
}

void LightingManager::toggleShadow() {
	shadow = !shadow;
}

bool LightingManager::getIfShadow() {
	return shadow;
}

bool* LightingManager::getIfShadow_Pointer() {
	return &shadow;
}

void LightingManager::setPostProcessNum(const int& num) {
	postProcessNum = num;
}
int LightingManager::getPostProcessNum() {
	return postProcessNum;
}

void LightingManager::moveLightForward(const float& dt_secs, const glm::vec3& cameraAt) {
	glm::vec3 dir = cameraAt * 75.0f * dt_secs;
	pointLights.at(0).position.x += dir.x;
	pointLights.at(0).position.z += dir.z;	
}

void LightingManager::moveLightBackward(const float& dt_secs, const glm::vec3& cameraAt) {
	glm::vec3 dir = cameraAt * 75.0f * dt_secs;
	pointLights.at(0).position.x -= dir.x;
	pointLights.at(0).position.z -= dir.z;
}

void LightingManager::moveLightLeft(const float& dt_secs, const glm::vec3& cameraAt, const glm::vec3& cameraUp) {
	glm::vec3 dir = glm::normalize(glm::cross(cameraAt, cameraUp)) * 75.0f * dt_secs;
	pointLights.at(0).position.x -= dir.x;
	pointLights.at(0).position.z -= dir.z;
}

void LightingManager::moveLightRight(const float& dt_secs, const glm::vec3& cameraAt, const glm::vec3& cameraUp) {
	glm::vec3 dir = glm::normalize(glm::cross(cameraAt, cameraUp)) * 75.0f * dt_secs;
	pointLights.at(0).position.x += dir.x;
	pointLights.at(0).position.z += dir.z;
}

void LightingManager::moveLightUp(const float& dt_secs) {
	float movement = 75.0f * dt_secs;
	pointLights.at(0).position.y += movement;
}

void LightingManager::moveLightDown(const float& dt_secs) {
	float movement = 75.0f * dt_secs;
	pointLights.at(0).position.y -= movement;
}


void LightingManager::setConstant(const bool& increase) {
	increase ? pointLights.at(0).att_constant += valueChangeRate : pointLights.at(0).att_constant -= valueChangeRate;
}
void LightingManager::setLinear(const bool& increase) {
	increase ? pointLights.at(0).att_linear += valueChangeRate : pointLights.at(0).att_linear -= valueChangeRate;
}
void LightingManager::setQuadratic(const bool& increase) {
	increase ? pointLights.at(0).att_quadratic += valueChangeRate : pointLights.at(0).att_quadratic -= valueChangeRate;
}

void LightingManager::setAmbient(const bool& increase, const char& comp) {
	if (comp == 'x') {
		increase ? pointLights.at(0).ambient.x += valueChangeRate : pointLights.at(0).ambient.x -= valueChangeRate;
	}
	else if (comp == 'y') {
		increase ? pointLights.at(0).ambient.y += valueChangeRate : pointLights.at(0).ambient.y -= valueChangeRate;
	}
	else {
		increase ? pointLights.at(0).ambient.z += valueChangeRate : pointLights.at(0).ambient.z -= valueChangeRate;
	}
}
void LightingManager::setDiffuse(const bool& increase, const char& comp) {
	if (comp == 'x') {
		increase ? pointLights.at(0).diffuse.x += valueChangeRate : pointLights.at(0).diffuse.x -= valueChangeRate;
	}
	else if (comp == 'y') {
		increase ? pointLights.at(0).diffuse.y += valueChangeRate : pointLights.at(0).diffuse.y -= valueChangeRate;
	}
	else {
		increase ? pointLights.at(0).diffuse.z += valueChangeRate : pointLights.at(0).diffuse.z -= valueChangeRate;
	}
}
void LightingManager::setSpecular(const bool& increase, const char& comp) {
	if (comp == 'x') {
		increase ? pointLights.at(0).specular.x += valueChangeRate : pointLights.at(0).specular.x -= valueChangeRate;
	}
	else if (comp == 'y') {
		increase ? pointLights.at(0).specular.y += valueChangeRate : pointLights.at(0).specular.y -= valueChangeRate;
	}
	else {
		increase ? pointLights.at(0).specular.z += valueChangeRate : pointLights.at(0).specular.z -= valueChangeRate;
	}
}

void LightingManager::setRange(const bool& increase) {
	float rangeChangeRate = 5.0f;
	increase ? pointLights.at(0).range += rangeChangeRate : pointLights.at(0).range -= rangeChangeRate;
}

void LightingManager::displayLightDetails() {
	for (unsigned int i = 0; i < pointLights.size(); i++) {
		std::cout << "\nconst: " << pointLights.at(i).att_constant << ", lin: " << pointLights.at(i).att_linear << ", quad: " << pointLights.at(i).att_quadratic << std::endl;
		std::cout << "amb: (" << pointLights.at(i).ambient.x << ", " << pointLights.at(i).ambient.y << ", " << pointLights.at(i).ambient.z << ")" << std::endl;
		std::cout << "diff: (" << pointLights.at(i).diffuse.x << ", " << pointLights.at(i).diffuse.y << ", " << pointLights.at(i).diffuse.z << ")" << std::endl;
		std::cout << "spec: (" << pointLights.at(i).specular.x << ", " << pointLights.at(i).specular.y << ", " << pointLights.at(i).specular.z << ")" << std::endl;
	}
}

std::tuple<int, int, int> LightingManager::getClosestLights(const glm::vec3& pos) {
	int id1 = -1; 
	int id2 = -1;
	int id3 = -1;
	for (unsigned int i = 0; i < pointLights.size(); i++) {
		std::pair<int, float> lightDistToObj = std::make_pair(i, std::hypotf(pos.x - pointLights.at(i).position.x, pos.z - pointLights.at(i).position.z));
		if (lightDistToObj.second < pointLights.at(i).range) {
			if (id1 < 0)
				id1 = pointLights.at(i).id;
			else if (id2 < 0)
				id2 = pointLights.at(i).id;
			else if (id3 < 0)
				id3 = pointLights.at(i).id;
			else {
				std::pair<int, float> lightDistToObj1 = std::make_pair(id1, std::hypotf(pos.x - pointLights.at(id1).position.x, pos.z - pointLights.at(id1).position.z));
				std::pair<int, float> lightDistToObj2 = std::make_pair(id2, std::hypotf(pos.x - pointLights.at(id2).position.x, pos.z - pointLights.at(id2).position.z));
				std::pair<int, float> lightDistToObj3 = std::make_pair(id3, std::hypotf(pos.x - pointLights.at(id3).position.x, pos.z - pointLights.at(id3).position.z));
				
				std::pair<int, float> dists[4]{ lightDistToObj, lightDistToObj1, lightDistToObj2, lightDistToObj3 };
				for (int it = 0; it < 4; it++) {
					for (int itt = it + 1; itt < 4; itt++) {
						if (dists[itt].second < dists[it].second) {
							std::pair<int, float> tempToAdd = dists[itt];
							std::pair<int, float> tempToChange = dists[it];
							dists[it] = tempToAdd;
							dists[itt] = tempToChange;
						}
					}
				}
				//pair add??

				id1 = dists[0].first;
				id2 = dists[1].first;
				id3 = dists[2].first;
			}
		}		
	}

	return std::tuple<int, int, int>(id1, id2, id3);
}

std::tuple<int, int, int> LightingManager::getClosestSpotLights(const glm::vec3& pos) {
	int id1 = -1;
	int id2 = -1;
	int id3 = -1;
	

	return std::tuple<int, int, int>(id1, id2, id3);
}