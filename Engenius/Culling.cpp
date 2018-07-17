#include "Culling.h"

void Plane::set3Points(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	
	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = aux2 * aux1;

	normal = glm::normalize(normal);
	point = v2;
	d = -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
}

void Plane::setNormalAndPoint(const glm::vec3& normal, const glm::vec3& point) {

	this->normal = normal;
	this->normal = glm::normalize(this->normal);
	d = -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
}

void FrustumCulling::setCamInternals(const float& angle, const float& ratio, const float& nearD, const float& farD) {

	// store the information
	this->ratio = ratio;
	this->angle = angle * HALF_ANG2RAD;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = tan(this->angle);
	sphereFactorY = 1.0f / cos(this->angle);//tang * sin(this->angle) + cos(this->angle);

	float anglex = atan(tang*ratio);
	sphereFactorX = 1.0f / cos(anglex); //tang*ratio * sin(anglex) + cos(anglex);

	nh = nearD * tang;
	nw = nh * ratio;

	fh = farD * tang;
	fw = fh * ratio;

}

void FrustumCulling::setCamDef(const glm::vec3& cameraEye, const glm::vec3& cameraAt, const glm::vec3& cameraUp) {

	glm::vec3 dir, nc, fc;

	camPos = cameraEye;

	// compute the Z axis of camera
	Z = cameraEye - cameraAt;
	Z = glm::normalize(Z);

	// X axis of camera of given "up" vector and Z axis
	X = glm::cross(Z, cameraUp);
	X = glm::normalize(X);

	// the real "up" vector is the cross product of Z and X
	Y = glm::cross(Z, X);

	// compute the center of the near and far planes
	nc = cameraEye - Z * nearD;
	fc = cameraEye - Z * farD;

	// compute the 8 corners of the frustum
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;

	// compute the six planes
	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	//	pl[NEARP].set3Points(ntl,ntr,nbr);
	//	pl[FARP].set3Points(ftr,ftl,fbl);

	pl[NEARP].setNormalAndPoint(-Z, nc);
	pl[FARP].setNormalAndPoint(Z, fc);

	glm::vec3 aux, normal;

	aux = (nc + Y*nh) - cameraEye;
	normal = aux * X;
	pl[TOP].setNormalAndPoint(normal, nc + Y*nh);

	aux = (nc - Y*nh) - cameraEye;
	normal = X * aux;
	pl[BOTTOM].setNormalAndPoint(normal, nc - Y*nh);

	aux = (nc - X*nw) - cameraEye;
	normal = aux * Y;
	pl[LEFT].setNormalAndPoint(normal, nc - X*nw);

	aux = (nc + X*nw) - cameraEye;
	normal = Y * aux;
	pl[RIGHT].setNormalAndPoint(normal, nc + X*nw);
}

int FrustumCulling::pointInFrustum(const glm::vec3& p) {

	float pcz, pcx, pcy, aux;

	// compute vector from camera position to p
	glm::vec3 v = p - camPos;

	// compute and test the Z coordinate
	pcz = v.x * -Z.x + v.y * -Z.y + v.z * -Z.z;
	if (pcz > farD || pcz < nearD)
		return(OUTSIDE);

	// compute and test the Y coordinate
	pcy = v.x * Y.x + v.y * Y.y + v.z * Y.z;
	aux = pcz * tang;
	if (pcy > aux || pcy < -aux)
		return(OUTSIDE);

	// compute and test the X coordinate
	pcx = v.x * X.x + v.y * X.y + v.z * X.z;
	aux = aux * ratio;
	if (pcx > aux || pcx < -aux)
		return(OUTSIDE);

	return(INSIDE);
}

int FrustumCulling::sphereInFrustum(const glm::vec3& p, const float& radius) {
	float d1, d2;
	float az, ax, ay, zz1, zz2;
	int result = INSIDE;

	glm::vec3 v = p - camPos;

	az = v.x * -Z.x + v.y * -Z.y + v.z * -Z.z;
	if (az > farD + radius || az < nearD - radius)
		return(OUTSIDE);

	ax = v.x * X.x + v.y * X.y + v.z * X.z;
	zz1 = az * tang * ratio;
	d1 = sphereFactorX * radius;
	if (ax > zz1 + d1 || ax < -zz1 - d1)
		return(OUTSIDE);

	ay = v.x * Y.x + v.y * Y.y + v.z * Y.z;
	zz2 = az * tang;
	d2 = sphereFactorY * radius;
	if (ay > zz2 + d2 || ay < -zz2 - d2)
		return(OUTSIDE);

	if (az > farD - radius || az < nearD + radius)
		result = INTERSECT;
	if (ay > zz2 - d2 || ay < -zz2 + d2)
		result = INTERSECT;
	if (ax > zz1 - d1 || ax < -zz1 + d1)
		result = INTERSECT;

	return(result);
}