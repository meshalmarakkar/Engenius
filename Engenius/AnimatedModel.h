#ifndef ANIMATEDMODEL
#define ANIMATEDMODEL

#include <unordered_map>
#include <vector>
#include "Model.h"

class AnimatedModel : public Model {
public:
	AnimatedModel(const string path, const bool justAnimation);
	AnimatedModel(std::string name, const string path);

	void BoneTransform(const float TimeInSeconds, vector<Matrix4f>& Transforms);
	std::pair<std::unordered_map<string, aiNodeAnim*>, std::pair<float, float>> getAnims();
	void setMovementAnims(const std::pair<std::unordered_map<string, aiNodeAnim*>, std::pair<float, float>> animationDetails);
	void getBoneDetails();
	unsigned int getNumBones();
	std::pair<glm::vec4, glm::vec3> getRightHandPos();

	void idleAnimation();
	void walkAnimation();
	void jogAnimation();
	void runAnimation();
	void walkBackAnimation();

private:
	void coutDetails(const aiNode* node);

	void InterpolateAnimations_ROT(aiQuaternion& Out, const float AnimationTime, const unsigned int mNumKeys, const aiQuatKey *mKeys, const unsigned int mNumKeys2, const aiQuatKey *mKeys2);
	void CalcInterpolated_ROT(aiQuaternion& Out, const float AnimationTime, const unsigned int mNumKeys, const aiQuatKey *mKeys);
	void InterpolateAnimations_POS(aiVector3D& Out, const float AnimationTime, const unsigned int mNumKeys, const aiVectorKey *mKeys, const unsigned int mNumKeys2, const aiVectorKey *mKeys2);
	void CalcInterpolated_POSorSCALE(aiVector3D& Out, const float AnimationTime, const unsigned int mNumKeys, const aiVectorKey *mKeys);

	void ReadNodeHeirarchy(const float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);

	float animationTime;

	struct Animation {
		std::unordered_map<string, aiNodeAnim*> animationNodes;
		std::pair<float, float> timeDetails;
	};
	Animation idle;
	std::vector<Animation> movementCycle;
	Animation *currentAnimation;
	Animation *nextAnimation;

	bool swapping;
	float blendFactor;
	const float BLEND_RATE = 0.15f;

	void beginAnimationSwap(Animation &nextAnim);
	void loadAnimation();

	Matrix4f m_GlobalInverseTransform;
};

#endif