#include "ParticleManager.h"
#include "TextureLoader.h"
#include <iostream>
ParticleManager::~ParticleManager()
{
	delete particleContainer;
	delete particleTextures;
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vboVertices);
	glDeleteBuffers(1, &vboPositions);
	glDeleteBuffers(1, &vboUVs);
	glDeleteBuffers(1, &vboBlend);
}

void ParticleManager::newToRender(ToRender &tr) {
	tr.positions = new std::vector<glm::vec3>;
	tr.UVs = new std::vector<glm::vec4>;
	tr.blend = new std::vector<float>;
	tr.positions->resize(MAX_PER_TYPE);
	tr.UVs->resize(MAX_PER_TYPE);
	tr.blend->resize(MAX_PER_TYPE);
}

ParticleManager::ParticleManager(unsigned int shader) : shader(shader) {

	particleTextures = new ParticleTexture[4]; //the wrong heap size thing!!!

	particleTextures[FIRE].texture = TextureLoader::loadTexture("firetorch2.png", "../Engenius/Textures/");
	particleTextures[FIRE].num_of_rows = 8;
	particleTextures[FIRE].optimumSize = 1.5f;
	particleTextures[FIRE].optimumLifeLength = 3.0f;
	particleTextures[FIRE].additive = false;

	particleTextures[EXPLOSION].texture = TextureLoader::loadTexture("Explosion.png", "../Engenius/Textures/");
	particleTextures[EXPLOSION].num_of_rows = 8;
	particleTextures[EXPLOSION].optimumSize = 2.5f;
	particleTextures[EXPLOSION].optimumLifeLength = 2.0f;
	particleTextures[EXPLOSION].additive = false;

	particleTextures[EXPLOSION2].texture = TextureLoader::loadTexture("fire.png", "../Engenius/Textures/");
	particleTextures[EXPLOSION2].num_of_rows = 8;
	particleTextures[EXPLOSION2].optimumSize = 2.5f;
	particleTextures[EXPLOSION2].optimumLifeLength = 3.0f;
	particleTextures[EXPLOSION2].additive = false;

	particleTextures[SMOKE].texture = TextureLoader::loadTexture("smoke.png", "../Engenius/Textures/");
	particleTextures[SMOKE].num_of_rows = 8;
	particleTextures[SMOKE].optimumSize = 2.5f;
	particleTextures[SMOKE].optimumLifeLength = 3.0f;
	particleTextures[SMOKE].additive = false;

	particleContainer = new Particle[MAX_PARTICLES];
	particleToRender = new std::unordered_map<ParticleType, ToRender>;
	ToRender tr;
	newToRender(tr);
	particleToRender->insert(std::make_pair(FIRE, tr));
	newToRender(tr);
	particleToRender->insert(std::make_pair(EXPLOSION, tr));
	newToRender(tr);
	particleToRender->insert(std::make_pair(EXPLOSION2, tr));
	newToRender(tr);
	particleToRender->insert(std::make_pair(SMOKE, tr));


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // bind VAO 0 as current object

	glGenBuffers(1, &vboVertices); //generate three buffers
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES) * sizeof(glm::vec2), VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glGenBuffers(1, &vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
	glBufferData(GL_ARRAY_BUFFER, MAX_PER_TYPE * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &vboUVs);
	glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
	glBufferData(GL_ARRAY_BUFFER, MAX_PER_TYPE * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &vboBlend);
	glBindBuffer(GL_ARRAY_BUFFER, vboBlend);
	glBufferData(GL_ARRAY_BUFFER, MAX_PER_TYPE * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);
}

void ParticleManager::SortParticles() {
	std::sort(&particleContainer[0], &particleContainer[particlesInUse]);
}

void ParticleManager::updateRepeated(Particle &p, float dt, glm::vec3 cameraPos) {

	ParticleTexture &texure = particleTextures[p.particleType];

	const int TEX_IN_LINE = texure.num_of_rows; // textures in each row or column 
	const int stageCount = TEX_IN_LINE * TEX_IN_LINE;
	const int FINAL_TEX_POS = TEX_IN_LINE - 1; // (7,7) is top left of the last image
	const float TEXTURES_IN_A_LINE = static_cast<float>(TEX_IN_LINE);

	float prog = 0.0f * stageCount;
	int beginning = static_cast<int>(floor(prog)); //where in atlas 

	if (p.elapesedTime + dt > texure.optimumLifeLength) {
		if (!p.dead)
			p.blend = 0.0f;
		p.dead = true;
	}
	else
		p.elapesedTime += dt;

	float lifeFactor = p.elapesedTime / texure.optimumLifeLength;

	float atlasProgression = lifeFactor * stageCount;

	int parser = static_cast<int>(floor(atlasProgression)); //where in atlas 
	int column = (parser % TEX_IN_LINE);
	int row = (parser / TEX_IN_LINE);
	float uv_x = column / TEXTURES_IN_A_LINE;
	float uv_y = row / TEXTURES_IN_A_LINE;
	p.texOffsets.x = uv_x;//top-left of parser
	p.texOffsets.y = uv_y;

	int upcomingParse = parser < stageCount - 1 ? parser + 1 : beginning; //not at end then check for next to blend
	column = (upcomingParse % TEX_IN_LINE);
	row = (upcomingParse / TEX_IN_LINE);
	uv_x = column / TEXTURES_IN_A_LINE;
	uv_y = row / TEXTURES_IN_A_LINE;
	p.texOffsets.z = uv_x;//top-left of parser
	p.texOffsets.w = uv_y;

	if (p.dead) {
		p.blend += 0.25f;
		if (p.blend == 0.75f) {
			p.elapesedTime = 0.0f;
			p.dead = false;
		}
	}
	else {
		p.blend = atlasProgression - static_cast<int>(atlasProgression);
	}

	p.cameraDistance = glm::length2(p.position - cameraPos);
}

void ParticleManager::updateUnrepeated(Particle &p, float dt, glm::vec3 cameraPos) {

	ParticleTexture &texure = particleTextures[p.particleType];

	if (p.elapesedTime + dt > texure.optimumLifeLength) {
		p.dead = true;
	}
	else
		p.elapesedTime += dt;
	
	if (!p.dead) {
		const int TEX_IN_LINE = texure.num_of_rows; // textures in each row or column 
		const int stageCount = TEX_IN_LINE * TEX_IN_LINE;
		const int FINAL_TEX_POS = TEX_IN_LINE - 1; // (7,7) is top left of the last image
		const float TEXTURES_IN_A_LINE = static_cast<float>(TEX_IN_LINE);

		float lifeFactor = p.elapesedTime / texure.optimumLifeLength;
		float atlasProgression = lifeFactor * stageCount;
		p.blend = atlasProgression - static_cast<int>(atlasProgression);

		int parser = static_cast<int>(floor(atlasProgression)); //where in atlas 
		int column = (parser % TEX_IN_LINE);
		int row = (parser / TEX_IN_LINE);
		float uv_x = column / TEXTURES_IN_A_LINE;
		float uv_y = row / TEXTURES_IN_A_LINE;
		p.texOffsets.x = uv_x;//top-left of parser
		p.texOffsets.y = uv_y;

		int upcomingParse = parser < stageCount - 1 ? parser + 1 : parser; //not at end then check for next to blend
		column = (upcomingParse % TEX_IN_LINE);
		row = (upcomingParse / TEX_IN_LINE);
		uv_x = column / TEXTURES_IN_A_LINE;
		uv_y = row / TEXTURES_IN_A_LINE;
		p.texOffsets.z = uv_x;//top-left of parser
		p.texOffsets.w = uv_y;

		p.position.x += p.velocity.x * dt;
		p.position.y += p.velocity.y * dt;
		p.position.z += p.velocity.z * dt;
		p.cameraDistance = glm::length2(p.position - cameraPos);
	}
}

void ParticleManager::Update(float dt, glm::vec3 cameraPos) {
	if (particlesInUse > 0) {

		for (int i = 0; i < particlesInUse; i++) {
			Particle &p = particleContainer[i];
			if (p.repeatedAnimation)
				updateRepeated(p, dt, cameraPos);
			else
				updateUnrepeated(p, dt, cameraPos);
		}

		SortParticles(); //sort so blend artifacts dont appear. rendered back to front.

		int numOfParticles = particlesInUse; //so for loop isnt interrupted
		for (int i = 0; i < numOfParticles; i++)
		{
			Particle &p = particleContainer[i];
			if (!p.repeatedAnimation) {
				if (p.dead == true) {
					p = particleContainer[particlesInUse - 1];
					particlesInUse--;
				}
			}

			particleToRender->at(p.particleType).positions->push_back(p.position);
			particleToRender->at(p.particleType).UVs->push_back(p.texOffsets);
			particleToRender->at(p.particleType).blend->push_back(p.blend);
		}
	}
}

void ParticleManager::renderParticles(glm::mat4 view, glm::mat4 projection) {
	if (particlesInUse > 0) {
		glUseProgram(shader);
		glUniform3f(glGetUniformLocation(shader, "CameraRight_worldspace"), view[0][0], view[1][0], view[2][0]);
		glUniform3f(glGetUniformLocation(shader, "CameraUp_worldspace"), view[0][1], view[1][1], view[2][1]);
		glm::mat4 ViewProjectionMatrix = projection * view;
		glUniformMatrix4fv(glGetUniformLocation(shader, "VP"), 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		glBindVertexArray(vao);

		for (std::unordered_map<ParticleType, ToRender>::iterator it = particleToRender->begin(); it != particleToRender->end(); ++it) {
			unsigned int numOfCurrentType = it->second.positions->size();
			ParticleTexture currentTexture = particleTextures[it->first];
			if (numOfCurrentType > 0) {

				if (currentTexture.additive)
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				else
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, currentTexture.texture);
				glUniform1i(glGetUniformLocation(shader, "textureMap"), 0);
				glUniform1f(glGetUniformLocation(shader, "size"), currentTexture.optimumSize);
				glUniform1ui(glGetUniformLocation(shader, "number_of_rows"), currentTexture.num_of_rows);

				Common::enableVertexAttribArray(0,3);
							
				glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
				glBufferSubData(GL_ARRAY_BUFFER, 0, numOfCurrentType * sizeof(glm::vec3), &it->second.positions->at(0));
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				
				glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
				glBufferSubData(GL_ARRAY_BUFFER, 0, numOfCurrentType * sizeof(glm::vec4), &it->second.UVs->at(0));
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

				glBindBuffer(GL_ARRAY_BUFFER, vboBlend);
				glBufferSubData(GL_ARRAY_BUFFER, 0, numOfCurrentType * sizeof(GL_FLOAT), &it->second.blend->at(0));
				glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

				glVertexAttribDivisor(0, 0); // VERTICES : always reuse the same 4 VERTICES -> 0
				glVertexAttribDivisor(1, 1); // positions : one per quad -> 1
				glVertexAttribDivisor(2, 1);
				glVertexAttribDivisor(3, 1);

				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, sizeof(VERTICES), numOfCurrentType);

				it->second.positions->clear();
				it->second.UVs->clear();
				it->second.blend->clear();
			}
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		Common::disableVertexAttribArray(0, 3);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	//	Common::unbindTextures(0);
	}
}

void ParticleManager::insertParticleGenerator(ParticleType particleType, bool repeatedAnimation, int numOfParticles, glm::vec3 position) {
	int increase = 0;
	for (int i = particlesInUse; i < (particlesInUse + numOfParticles); i++) {
		Particle &p = particleContainer[i];

		p.particleType = particleType;
		p.repeatedAnimation = repeatedAnimation;
		p.dead = false;
		p.generationPosition = position;
		p.position = position;
		p.velocity.x = static_cast<float>(std::rand() % 2 - 1);
		//particleContainer[i].velocities.y = (std::rand() % 100 - 50) / 10.0f;
		p.velocity.y = 2.0f;
		p.velocity.z = static_cast<float>(std::rand() % 2 - 1);
		p.elapesedTime = 0.0f;
		p.cameraDistance = -1.0f;
		p.blend = 0.0f;
		p.texOffsets = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		increase++;
	}
	particlesInUse += increase;
}

void ParticleManager::insertFire(int numOfParticles, glm::vec3 position, bool repeatedAnimation) {
	insertParticleGenerator(FIRE, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::insertExplosion(int numOfParticles, glm::vec3 position, bool repeatedAnimation) {
	insertParticleGenerator(EXPLOSION, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::insertExplosion2(int numOfParticles, glm::vec3 position, bool repeatedAnimation) {
	insertParticleGenerator(EXPLOSION2, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::insertSmoke(int numOfParticles, glm::vec3 position, bool repeatedAnimation) {
	insertParticleGenerator(SMOKE, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::moveParticleManually(glm::vec3 newPos) {
	particleContainer[0].position = newPos;
}