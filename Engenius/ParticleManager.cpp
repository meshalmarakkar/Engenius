#include "ParticleManager.h"
#include "TextureLoader.h"
#include <iostream>
ParticleManager::~ParticleManager()
{
	delete particleContainer;
	delete particleTextures;
	//glDeleteBuffers(1, &vao);
	//glDeleteBuffers(1, &vboVertices);
	//glDeleteBuffers(1, &vboPositions);
	//glDeleteBuffers(1, &vboUVs);
	//glDeleteBuffers(1, &vboBlend);
}

void ParticleManager::newToRender(ToRender &tr) {
	tr.positions = new std::vector<glm::vec3>;
	tr.UVs = new std::vector<glm::vec4>;
	tr.blend = new std::vector<float>;
	tr.positions->reserve(MAX_PER_TYPE);
	tr.UVs->reserve(MAX_PER_TYPE);
	tr.blend->reserve(MAX_PER_TYPE);
}

ParticleManager::ParticleManager(Shader* shader) : shaderProgram(shader) {

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

	VAO = new VertexArrayObject(6, Buffer_Options::gl_triangle_strip);

	vboVertices = VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, sizeof(VERTICES) * sizeof(glm::vec2), VERTICES, Buffer_Options::gl_static_draw);
	VAO->enableVertexAttribArray(0, 2, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	vboPositions = VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, MAX_PER_TYPE * sizeof(glm::vec3), NULL, Buffer_Options::gl_stream_draw);
	VAO->enableVertexAttribArray(1, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	vboUVs = VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, MAX_PER_TYPE * sizeof(glm::vec4), NULL, Buffer_Options::gl_stream_draw);
	VAO->enableVertexAttribArray(2, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	vboBlend = VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, MAX_PER_TYPE * sizeof(GL_FLOAT), NULL, Buffer_Options::gl_stream_draw);
	VAO->enableVertexAttribArray(3, 1, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	VAO->setDivisor(vboVertices, 0);
	VAO->setDivisor_multi(vboPositions, vboBlend, 1);

	insertSmoke(1, glm::vec3(0.0f, 2.0f, 0.0f), true);
	insertSmoke(1, glm::vec3(1.0f, 2.0f, 0.0f), true);
	insertSmoke(1, glm::vec3(-1.0f, 2.0f, 0.0f), true);

	//particleManager->insertFire(1, glm::vec3(0.0f, 4.25f, 37.5f), true);
	//particleManager->insertFire(1, glm::vec3(1.0f, 4.25f, 37.5f), true);
	//particleManager->insertFire(1, glm::vec3(-1.0f, 4.25f, 37.5f), true);
	//particleManager->insertFire(1, glm::vec3(-1.0f, 4.25f, 38.5f), true);
	//particleManager->insertFire(1, glm::vec3(-1.0f, 4.25f, 36.5f), true);
}

void ParticleManager::SortParticles() {
	std::sort(&particleContainer[0], &particleContainer[particlesInUse]);
}

void ParticleManager::updateRepeated(Particle &p, const float& dt, const glm::vec3& cameraPos) {

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

void ParticleManager::updateUnrepeated(Particle &p, const float& dt, const glm::vec3& cameraPos) {

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

void ParticleManager::Update(const float& dt, const glm::vec3& cameraPos) {
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

			particleToRender->at(p.particleType).positions->emplace_back(p.position);
			particleToRender->at(p.particleType).UVs->emplace_back(p.texOffsets);
			particleToRender->at(p.particleType).blend->emplace_back(p.blend);
		}
	}
}

void ParticleManager::renderParticles(const glm::mat4& view, const glm::mat4& projection, Renderer* renderer) {
	if (particlesInUse > 0) {
		shaderProgram->bind();
		shaderProgram->uniform("CameraRight_worldspace", glm::vec3(view[0][0], view[1][0], view[2][0]));
		shaderProgram->uniform("CameraUp_worldspace", glm::vec3(view[0][1], view[1][1], view[2][1]));
		shaderProgram->uniform("VP", projection * view);

		renderer->enableBlend();
		renderer->depthMask_false();

		VAO->bind();

		for (std::unordered_map<ParticleType, ToRender>::iterator it = particleToRender->begin(); it != particleToRender->end(); ++it) {
			unsigned int numOfCurrentType = it->second.positions->size();
			ParticleTexture currentTexture = particleTextures[it->first];
			if (numOfCurrentType > 0) {

				if (currentTexture.additive)
					renderer->setBlendFunction(BlendOptions::gl_one);
				else
					renderer->setBlendFunction(BlendOptions::gl_one_minus_src_alpha);

				shaderProgram->bindTex_2D("textureMap", currentTexture.texture);
				shaderProgram->uniform("size", currentTexture.optimumSize);
				shaderProgram->uniform("number_of_rows", currentTexture.num_of_rows);

				VAO->updateData(vboPositions, Buffer_Options::gl_array_buffer, numOfCurrentType * sizeof(glm::vec3), &it->second.positions->at(0));
				VAO->updateData(vboUVs, Buffer_Options::gl_array_buffer, numOfCurrentType * sizeof(glm::vec4), &it->second.UVs->at(0));
				VAO->updateData(vboBlend, Buffer_Options::gl_array_buffer, numOfCurrentType * sizeof(GL_FLOAT), &it->second.blend->at(0));

				renderer->drawArrays_instanced(VAO, numOfCurrentType);

				it->second.positions->clear();
				it->second.UVs->clear();
				it->second.blend->clear();
			}
		}

		renderer->depthMask_true();
		renderer->disableBlend();
	}
}

void ParticleManager::insertParticleGenerator(ParticleType particleType, const bool& repeatedAnimation, const int& numOfParticles, const glm::vec3& position) {
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

void ParticleManager::insertFire(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation) {
	insertParticleGenerator(FIRE, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::insertExplosion(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation) {
	insertParticleGenerator(EXPLOSION, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::insertExplosion2(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation) {
	insertParticleGenerator(EXPLOSION2, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::insertSmoke(const int& numOfParticles, const glm::vec3& position, const bool& repeatedAnimation) {
	insertParticleGenerator(SMOKE, repeatedAnimation, numOfParticles, position);
}

void ParticleManager::moveParticleManually(const glm::vec3& newPos) {
	particleContainer[0].position = newPos;
}