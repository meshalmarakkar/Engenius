#ifndef CULLING
#define CULLING

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class Plane {
public:
	Plane::Plane() { ; }
	Plane::~Plane() { ; }
	void set3Points(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3);
	void setNormalAndPoint(const glm::vec3 normal, const glm::vec3 point);

private:
	glm::vec3 normal;
	float d;
	glm::vec3 point;

};

class FrustumCulling {
public:
	FrustumCulling() { pl = new Plane[NUMPLANES]; }
	FrustumCulling::~FrustumCulling() {}
	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(const glm::vec3 cameraEye, const glm::vec3 cameraAt, const glm::vec3 cameraUp);
	int pointInFrustum(const glm::vec3 p);
	int sphereInFrustum(const glm::vec3 p, const float rad);

private:
	const float HALF_ANG2RAD = 3.14159265358979323846f/360.0f;

	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr, X, Y, Z, camPos;
	float nearD, farD, ratio, angle;
	float sphereFactorX, sphereFactorY;
	float tang;
	float nw, nh, fw, fh;

	enum { OUTSIDE, INTERSECT, INSIDE };
	const int NUMPLANES = 6;
	Plane* pl;
	enum planes {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

};

#endif