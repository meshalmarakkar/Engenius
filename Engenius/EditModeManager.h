#ifndef EDITMODEMANAGER
#define EDITMODEMANAGER

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SDL.h"

#include "Common.h"
#include "Model.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "ParticleManager.h"
#include "HUDManager.h"
#include "LightingManager.h"
#include "AudioManager.h"
#include "ShaderManager.h"

class EditModeManager {
public:
	EditModeManager(EntityManager* entityManager, CollisionManager* colManager, ParticleManager* particleManager, HUDManager* hudManager, LightingManager* lightingManager, AudioManager* audioManager, ShaderManager* shaderManager);

	void update(const glm::vec3 currentTerrainPointOfMouse, float dt_secs);
	void draw(GLuint shader);

	int getViewMode();
	void toggleViewMode();

	void switchModel(bool next);
	void edit_SetPos(glm::vec3 pos);
	void edit_SetScale(bool increase);
	void edit_SetScaleX(bool increase);
	void edit_SetScaleY(bool increase);
	void edit_SetScaleZ(bool increase);
	void edit_SetYaw(bool increase);

	void edit_SetPitch90(bool increase);
	void edit_SetYaw90(bool increase);
	void edit_SetRoll90(bool increase);

	void edit_ScaleRate(bool increase);

	void toggleEditMode();
	bool get_ifEditMode();

	void toggleAddMode();
	bool get_ifAddMode();

	void toggleMoveMode();
	bool get_ifMoveMode();

	void toggleVisualize();
	bool get_ifVisualize();

	bool get_ifControlPlayerMode();
	void toggle_controlPlayer();

	bool get_ifBoundingMode();
	bool get_ifObjectsMode();
	bool get_ifLightingMode();
	bool get_ifAudioMode();

	void set_ObjectsMode();
	void set_BoundingMode();
	void set_LightingMode();
	void set_AudioMode();
	void toggleModes();

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

	void edit_SetYOffset(bool increase);

	void edit_GridNo(bool increase);

	void addFunction();
	void undoFunction();
	
	void setIndexOfSelectedObject(unsigned int index);

private:
	EntityManager * entityManager;
	CollisionManager * colManager;
	ParticleManager * particleManager;
	HUDManager* hudManager;
	LightingManager* lightingManager;
	AudioManager* audioManager;
	ShaderManager* shaderManager;

	void init();
	void recalcSides_Grid();

	std::unordered_map<std::string, Model*>::iterator edit_ModelIterator;
	std::vector<Entity*>::iterator edit_ObjectIterator;
	std::vector<HeirarchicalGrid*>::iterator edit_HeirIterator;
	std::vector<btRigidBody*>::iterator edit_RigidIterator;
	std::pair<int, int> edit_boundLocation;

	unsigned int indexOfSelectedObject;
	float scaleRate;

	struct Mode {
		bool addMode;
		bool moveMode;
		bool visualize;
		glm::vec3 edit_pos;
		glm::vec3 edit_scale;
		float edit_yaw;
		float edit_pitch;
		float edit_roll;

		Mode() {
			addMode = false;
			moveMode = false;
			visualize = false;
			edit_pos = glm::vec3(0.0f);
			edit_yaw = 0.0f;
			edit_scale = glm::vec3(0.1f);
			edit_pitch = 0.0f;
			edit_roll = 0.0f;
		 }
	};

	unsigned int gridNo;

	Mode* mode_bounding;
	Mode* mode_lights;
	Mode* mode_audio;
	Mode* mode_objects;

	Mode* mode_selected;
	void resetCurrentMode();

	bool editMode;
	bool bool_controlPlayer;
	
	int numMode;
	int viewMode;
	float edit_yOffset;
};
#endif
