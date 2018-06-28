#ifndef PARTICLEMANAGER
#define PARTICLEMANAGER

#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include <vector>
#include <glm/gtx/norm.hpp> //for glm::length2
#include "Common.h"
#include <math.h>
#include <unordered_map>

static const glm::vec2 VERTICES[] = {
	glm::vec2(-0.5f, 0.5f),
	glm::vec2(-0.5f, -0.5f),
	glm::vec2(0.5f, 0.5f),
	glm::vec2(0.5f, -0.5f),
	glm::vec2(0.5f, 0.5f),
	glm::vec2(-0.5f, -0.5f),
};


class ParticleManager {
public:
	ParticleManager(unsigned int shader);
	~ParticleManager();
	void Update(float dt, glm::vec3 cameraPos);

	void renderParticles(glm::mat4 view, glm::mat4 projection);
	void insertFire(int numOfParticles, glm::vec3 position, bool repeatedAnimation);
	void insertExplosion(int numOfParticles, glm::vec3 position, bool repeatedAnimation);
	void insertExplosion2(int numOfParticles, glm::vec3 position, bool repeatedAnimation);
	void insertSmoke(int numOfParticles, glm::vec3 position, bool repeatedAnimation);
	void moveParticleManually(glm::vec3 newPos);

private:
	enum ParticleType {
		FIRE = 0,
		EXPLOSION = 1,
		EXPLOSION2 = 2,
		SMOKE = 3
	};
	struct ParticleTexture {
		unsigned int texture = 0;
		unsigned int num_of_rows = 0;
		float optimumSize = 0.0f;
		float optimumLifeLength = 0.0f;
		bool additive;
	};

	struct Particle {
		ParticleType particleType;
		bool repeatedAnimation;
		bool dead;
		glm::vec3 generationPosition;
		glm::vec3 position;
		glm::vec3 velocity;
		float elapesedTime;
		float cameraDistance;
		float blend;//to blend smoothly from one to next;
		glm::vec4 texOffsets;//xy is currentParser, zw is nextBox
		bool additive;

		//to tell std::sort which way to sort
		bool operator<(const Particle & comparingParticle) const {
			// Sort in reverse order : far particles drawn first.
			return this->cameraDistance > comparingParticle.cameraDistance;
		}
	};

	void insertParticleGenerator(ParticleType particleType, bool repeatedAnimation, int numOfParticles, glm::vec3 position);
	void ParticleManager::updateRepeated(Particle &p, float dt, glm::vec3 cameraPos);
	void ParticleManager::updateUnrepeated(Particle &p, float dt, glm::vec3 cameraPos);
	void SortParticles();
	const int MAX_PARTICLES = 300;
	const int MAX_PER_TYPE = 20;
	int particlesInUse = 0;

	unsigned int shader;


	struct ToRender {
		std::vector<glm::vec3>* positions;
		std::vector<glm::vec4>* UVs;
		std::vector<float>* blend;
	};
	void newToRender(ToRender &tr);
	std::unordered_map<ParticleType, ToRender>* particleToRender;

	Particle * particleContainer;
	ParticleTexture * particleTextures;

	unsigned int vao;
	unsigned int vboVertices;
	unsigned int vboPositions;
	unsigned int vboUVs;
	unsigned int vboBlend;
};


//class ParticleManager {
//public:
//	ParticleManager();
//	void render(unsigned int shader, glm::mat4 view, glm::mat4 projection);
//
//private:
//	const float VERTICES[8] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
//	unsigned int vao, vboVertices, vboPositions, vboUVs;
//	const int MAX_PER_TYPE = 20;
//
//};


#endif
