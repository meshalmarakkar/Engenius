#pragma once

//#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "VertexArrayObject.h"
#include "Framebuffer.h"
#include "WindowManager.h"
//#include "Entity.h"
#include "ShaderManager.h"
//#include "TextureLoader.h"
#include "Material.h"

class Renderer {
public:
	Renderer(WindowManager* _windowManager, ShaderManager* _shaderManager);
	~Renderer();

	void setupFrame_Deferred();
	void setupFrame_PostProcess();

	void setup_Screen();

	void toShader_Buffers(GLuint shader);

	void SS_Bloom();
	void SS_GodRays();

	void drawArrays(VertexArrayObject* VAO);
	void drawElements(VertexArrayObject* VAO);
	void drawElements_w_primitiveRestart(GLuint shader, VertexArrayObject* VAO, Material* mat);
	void draw(VertexArrayObject* VAO);
	void draw_screenQuad();
	void draw_finalDisplay(const float exposure, const int ifBloom, const int effectNo, bool ifPause, GLuint tex_pause, bool ifEndGame, GLuint tex_end);


	void bindTexture(const unsigned int shader, const unsigned int texture, const char* name);
	void bindCubeMapTexture(const unsigned int shader, const unsigned int texture, const char* name);
	void unbindTextures(unsigned int num);
	void unbindTextures(unsigned int first, unsigned int last);
	void unbindAllTextures();

private:
	void initFBOs();
	void initScreen();



private:
	const float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								   // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	WindowManager* windowManager;
	ShaderManager* shaderManager;

	GLuint currentShader;

	VertexArrayObject* VAO_screenQuad;

	Framebuffer* FBO_postProcess;

	//Bloom - "ping pong" buffers to do horizontal and vertical blurring separately to save performance
	Framebuffer* FBO_pingpong[2];

	//Deferred Rendering
	Framebuffer* FBO_gBuffer;

	unsigned int num_nextAvailableTex;
};