#include "AnimatedModel.h" 

AnimatedModel::AnimatedModel(const string path, const bool justAnimation) : Model(path, justAnimation) {
	this->loadAnimation();
}

AnimatedModel::AnimatedModel(std::string name, const string path) : Model(name, path) {
	this->loadAnimation();
}

void AnimatedModel::loadAnimation() {
	if (scene->HasAnimations()) {
		for (unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
			idle.animationNodes.insert(make_pair(scene->mAnimations[0]->mChannels[i]->mNodeName.data, scene->mAnimations[0]->mChannels[i]));
		}

		idle.timeDetails = std::pair<float, float>(static_cast<float>(scene->mAnimations[0]->mTicksPerSecond), static_cast<float>(scene->mAnimations[0]->mDuration));
		m_GlobalInverseTransform = scene->mRootNode->mTransformation;
		currentAnimation = &idle;
		nextAnimation = &idle;
		blendFactor = BLEND_RATE;
		swapping = false;
		this->animationTime = 0.0f;
	}
}

std::pair<std::unordered_map<string, aiNodeAnim*>, std::pair<float, float>> AnimatedModel::getAnims() { return std::make_pair(idle.animationNodes, idle.timeDetails); }

void AnimatedModel::setMovementAnims(const std::pair<std::unordered_map<string, aiNodeAnim*>, std::pair<float, float>> animationDetails) {
	Animation movement;
	movement.animationNodes = animationDetails.first;
	movement.timeDetails = animationDetails.second;
	movementCycle.push_back(movement);
}

unsigned int FindPosOrScale(const float AnimationTime, const unsigned int mNumKeys, const aiVectorKey *mKeys)
{
	for (unsigned int i = 0; i < mNumKeys - 1; i++)
		if (AnimationTime < static_cast<float>(mKeys[i + 1].mTime))
			return i;
	assert(0);
	return 0;
}

unsigned int FindRotation(const float AnimationTime, const unsigned int mNumKeys, const aiQuatKey *mKeys)
{
	assert(mNumKeys > 0);
	for (unsigned int i = 0; i < mNumKeys - 1; i++)
		if (AnimationTime < static_cast<float>(mKeys[i + 1].mTime))
			return i;
	assert(0);
	return 0;
}

void AnimatedModel::CalcInterpolated_POSorSCALE(aiVector3D& Out, const float AnimationTime, const unsigned int mNumKeys, const aiVectorKey *mKeys)
{
	if (mNumKeys == 1) {
		Out = mKeys[0].mValue;
		return;
	}

	//to connect one keyframe to another, have to do it here
	unsigned int PositionIndex = FindPosOrScale(AnimationTime, mNumKeys, mKeys);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < mNumKeys);
	//time for next keyframe - time for current keyframe time
	float DeltaTime = (float)(mKeys[NextPositionIndex].mTime - mKeys[PositionIndex].mTime);
	// animation time - current keyframe time / delta
	float Factor = (AnimationTime - (float)mKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& Start = mKeys[PositionIndex].mValue;
	const aiVector3D& End = mKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void AnimatedModel::CalcInterpolated_ROT(aiQuaternion& Out, const float AnimationTime, const unsigned int mNumKeys, const aiQuatKey *mKeys)
{
	// we need at least two values to interpolate...
	if (mNumKeys == 1) {
		Out = mKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, mNumKeys, mKeys);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < mNumKeys);
	float DeltaTime = (float)(mKeys[NextRotationIndex].mTime - mKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)mKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiQuaternion& StartRotationQ = mKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = mKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void AnimatedModel::InterpolateAnimations_POS(aiVector3D& Out, const float AnimationTime, const unsigned int mNumKeys, const aiVectorKey *mKeys, const unsigned int mNumKeys2, const aiVectorKey *mKeys2)
{
	if (mNumKeys == 1) {
		Out = mKeys[0].mValue;
		return;
	}

	//to connect one keyframe to another, have to do it here
	unsigned int PositionIndex = FindPosOrScale(AnimationTime, mNumKeys, mKeys);
	unsigned int NextPositionIndex = 0;
	assert(NextPositionIndex < mNumKeys2);
	float Factor = blendFactor;

	const aiVector3D& Start = mKeys[PositionIndex].mValue;
	const aiVector3D& End = mKeys2[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void AnimatedModel::InterpolateAnimations_ROT(aiQuaternion& Out, const float AnimationTime, const unsigned int mNumKeys, const aiQuatKey *mKeys, const unsigned int mNumKeys2, const aiQuatKey *mKeys2)
{
	// we need at least two values to interpolate...
	if (mNumKeys == 1) {
		Out = mKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, mNumKeys, mKeys);
	unsigned int NextRotationIndex = 0;
	assert(NextRotationIndex < mNumKeys2);
	float Factor = blendFactor;

	const aiQuaternion& StartRotationQ = mKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = mKeys2[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void AnimatedModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
{
	string NodeName(pNode->mName.data);
	Matrix4f NodeTransformation(pNode->mTransformation);
	const aiNodeAnim* pNodeAnim = currentAnimation->animationNodes[NodeName];
	const aiNodeAnim* pNodeAnim2 = nextAnimation->animationNodes[NodeName];

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolated_POSorSCALE(Scaling, AnimationTime, pNodeAnim->mNumScalingKeys, pNodeAnim->mScalingKeys);
		Matrix4f ScalingM;
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		aiQuaternion RotationQ;
		aiVector3D Translation;
		if (swapping == true) {
			InterpolateAnimations_ROT(RotationQ, AnimationTime, pNodeAnim->mNumRotationKeys, pNodeAnim->mRotationKeys, pNodeAnim2->mNumRotationKeys, pNodeAnim2->mRotationKeys);
			InterpolateAnimations_POS(Translation, AnimationTime, pNodeAnim->mNumPositionKeys, pNodeAnim->mPositionKeys, pNodeAnim2->mNumPositionKeys, pNodeAnim2->mPositionKeys);
		}
		else {
			CalcInterpolated_ROT(RotationQ, AnimationTime, pNodeAnim->mNumRotationKeys, pNodeAnim->mRotationKeys);
			CalcInterpolated_POSorSCALE(Translation, AnimationTime, pNodeAnim->mNumPositionKeys, pNodeAnim->mPositionKeys);
		}

		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());
		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		//now that bone is transformed, do corresponding transforms for children
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void Matrix4f_to_GLMmat4(glm::mat4 &pos, const Matrix4f matpos) {
	pos[0][0] = matpos.m[0][0]; pos[1][0] = matpos.m[0][1]; pos[2][0] = matpos.m[0][2]; pos[3][0] = matpos.m[0][3];
	pos[0][1] = matpos.m[1][0]; pos[1][1] = matpos.m[1][1]; pos[2][1] = matpos.m[1][2]; pos[3][1] = matpos.m[1][3];
	pos[0][2] = matpos.m[2][0]; pos[1][2] = matpos.m[2][1]; pos[2][2] = matpos.m[2][2]; pos[3][2] = matpos.m[2][3];
	pos[0][3] = matpos.m[3][0]; pos[1][3] = matpos.m[3][1]; pos[2][3] = matpos.m[3][2]; pos[3][3] = matpos.m[3][3];
}

glm::vec3 getRotation(const glm::mat4 mat) {
	glm::length_t sy = glm::vec3(mat[1][0], mat[1][1], mat[1][2]).length();
	glm::length_t sz = glm::vec3(mat[2][0], mat[2][1], mat[2][2]).length();
	float j = mat[1][2] / sy;
	float k = mat[2][2] / sz;
	float pitch = atan2(j, k);
	return glm::vec3(pitch, 0.0f, 0.0f);

	//glm::length_t sy = glm::vec3(mat[0][1], mat[1][1], mat[2][1]).length();
	//glm::length_t sz = glm::vec3(mat[0][2], mat[1][2], mat[2][2]).length();
	//float j = mat[2][1] / sy;
	//float k = mat[2][2] / sz;
	//float pitch = atan2(j, k);
	//return glm::vec3(pitch, 0.0f, 0.0f);
}

std::pair<glm::vec4, glm::vec3> AnimatedModel::getRightHandPos() {
	Matrix4f matpos;
	unsigned int BoneIndex = m_BoneMapping["mixamorig_RightHandIndex1"];
	matpos = m_BoneInfo[BoneIndex].FinalTransformation;

	glm::mat4 pos;
	Matrix4f_to_GLMmat4(pos, matpos);

	glm::mat4 boneOffset;
	Matrix4f_to_GLMmat4(boneOffset, m_BoneInfo[BoneIndex].BoneOffset);

	glm::mat4 right = (pos * glm::inverse(boneOffset));
	std::pair<glm::vec4, glm::vec3> rightHandDetails;
	rightHandDetails.first = glm::vec4(right[3][0], right[3][1], right[3][2], 1.0f);
	rightHandDetails.second = getRotation(right);

	return rightHandDetails;
}

void AnimatedModel::coutDetails(const aiNode* pNode) {
	std::cout << pNode->mName.data << std::endl;
	unsigned int BoneIndex = m_BoneMapping[pNode->mName.data];
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			std::cout << m_BoneInfo[BoneIndex].FinalTransformation.m[x][y] << ", ";
		}
		std::cout << std::endl;
	}
	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		coutDetails(pNode->mChildren[i]);
	}
}

void AnimatedModel::getBoneDetails() {
	coutDetails(scene->mRootNode);
}

void AnimatedModel::beginAnimationSwap(Animation &nextAnim) {
	blendFactor = BLEND_RATE;
	swapping = true;
	this->nextAnimation = &nextAnim;
}

void AnimatedModel::idleAnimation() {
	beginAnimationSwap(idle);
}
void AnimatedModel::walkAnimation() {
	beginAnimationSwap(movementCycle[0]);
}
void AnimatedModel::jogAnimation() {
	beginAnimationSwap(movementCycle[1]);
}
void AnimatedModel::runAnimation() {
	beginAnimationSwap(movementCycle[2]);
}
void AnimatedModel::walkBackAnimation() {
	beginAnimationSwap(movementCycle[3]);
}

unsigned int AnimatedModel::getNumBones() { return m_NumBones; }

void AnimatedModel::BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms)
{
	Matrix4f Identity;
	Identity.InitIdentity();

	if (swapping == true) {
		if (blendFactor >= 1.0f) {
			blendFactor = BLEND_RATE;
			this->animationTime = 0.0f;
			currentAnimation = nextAnimation;
			swapping = false;
		}
		else
			blendFactor += BLEND_RATE;
	}
	else
		this->animationTime += TimeInSeconds;

	float animTPS = currentAnimation->timeDetails.first;
	float duration = currentAnimation->timeDetails.second;

	float TicksPerSecond = animTPS != 0 ? animTPS : 25.0f;
	float TimeInTicks = this->animationTime * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, duration); //TimeInTicks / Duration

	ReadNodeHeirarchy(AnimationTime, scene->mRootNode, Identity);

	for (unsigned int i = 0; i < m_NumBones; i++) {
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}