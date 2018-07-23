#ifndef PARTICLEMANAGER
#define PARTICLEMANAGER

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include <vector>
#include <glm/gtx/norm.hpp> //for glm::length2
#include "Common.h"
#include <math.h>
#include <unordered_map>

#include "VertexArrayObject.h"
#include "Shader.h"
#include "Renderer.h"

class ParticleManager {
public:
	ParticleManager(Shader* shader);
	~ParticleManager();
	void Update(const float& dt, const glm::vec3& cameraPos);

	void renderParticles(const glm::mat4& view, const glm::mat4& projection, Renderer* renderer);
	void insertFire(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation);
	void insertExplosion(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation);
	void insertExplosion2(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation);
	void insertSmoke(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation);
	void moveParticleManually(const glm::vec3& newPos);

private:
	const glm::vec2 VERTICES[6] = {
		glm::vec2(-0.5f, 0.5f),
		glm::vec2(-0.5f, -0.5f),
		glm::vec2(0.5f, 0.5f),
		glm::vec2(0.5f, -0.5f),
		glm::vec2(0.5f, 0.5f),
		glm::vec2(-0.5f, -0.5f),
	};
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

	void insertParticleGenerator(ParticleType particleType, const bool& repeatedAnimation, const int& numOfParticles, const glm::vec3& position);
	void ParticleManager::updateRepeated(Particle &p, const float& dt, const glm::vec3& cameraPos);
	void ParticleManager::updateUnrepeated(Particle &p, const float& dt, const glm::vec3& cameraPos);
	void SortParticles();

private:
	const int MAX_PARTICLES = 300;
	const int MAX_PER_TYPE = 20;
	int particlesInUse = 0;

	VertexArrayObject* VAO;
	unsigned int vboVertices;
	unsigned int vboPositions;
	unsigned int vboUVs;
	unsigned int vboBlend;

	Shader* shaderProgram;

	struct ToRender {
		std::vector<glm::vec3>* positions;
		std::vector<glm::vec4>* UVs;
		std::vector<float>* blend;
	};
	void newToRender(ToRender &tr);
	std::unordered_map<ParticleType, ToRender>* particleToRender;

	Particle * particleContainer;
	ParticleTexture * particleTextures;
};
#endif
