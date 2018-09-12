#define EDGE_BIAS 0.006f	//when hitpoint on edge or point
#define FACE_BIAS 0.05f
#include "../include/Ray.h"

Ray::Ray(): origin(glm::vec3(0,0,0)), direction(glm::vec3(0,0,-1)) {
	
}
Ray::Ray(glm::vec3 origin_, glm::vec3 direction_) : origin(origin_), direction(direction_) {

}
Ray::~Ray(){

}

bool Ray::intersectMesh(const MeshPool& meshPool, glm::vec3& fragPosition, glm::vec2& fragTexCoord, glm::vec3& fragNormal, Model* &modelPtr) const {
	
	//was a face hit
	bool hitFace = false;

	//distance to running closest face
	float minDist;

	//loop through all faces in model
	for (unsigned int currFace = 0; currFace < meshPool.allFaces.size(); ++currFace) {

		//used to find intersection position if ray hits polygon
		glm::vec3 currIntersection;

		//if ray hits triangle
		if (intersectTriangle(meshPool.allFaces.at(currFace), currIntersection)) {

			float currDist = glm::length(currIntersection - origin);

			if (currDist <= FACE_BIAS) {
				continue;
			}
			//if first time we hit a face, OR we hit a new closest face
			if (!hitFace || currDist < minDist) {
				fragPosition = currIntersection;
				fragNormal = meshPool.allFaces.at(currFace).interpNormal(currIntersection);
				fragTexCoord = meshPool.allFaces.at(currFace).interpTexCoord(currIntersection);
				modelPtr = meshPool.allFaces.at(currFace).getModel();
				minDist = currDist;
			}

			hitFace = true;
		}
	}

	//return whether or not a face was hit
	return hitFace;
}


bool Ray::intersectTriangle(const Triangle& triangle, glm::vec3& intersection) const {

	//if ray is perpendicular to normal, ray never hits triangle plane
	if (glm::dot(triangle.getFaceNormal(), direction) == 0) {
		return false;
	}

	//CODE FOR BACKFACE CULLING, TOGGLE AT WILL
	//if ray and triangle normal point in roughly same direction, they don't intersect
	//if(glm::dot(triangle.getFaceNormal(), direction) > 0){
	//	return false;
	//}

	glm::vec3 n =  triangle.getFaceNormal();
	glm::vec3 dir = glm::normalize(direction);
	float d = glm::dot(n, triangle.v1->position);

	float t = (d - glm::dot(n, origin)) / glm::dot(n,dir);

	//if t is negative, ray is going away from triangle
	if(t < 0){
		return false;
	}

	//calclate intersection position
	glm::vec3 Q = origin + t * dir;

	//run through conclusive cases of intersection point out of triangle
	if(glm::dot(glm::cross(triangle.v2->position - triangle.v1->position, Q - triangle.v1->position), n) < -EDGE_BIAS){
		return false;
	}
	if(glm::dot(glm::cross(triangle.v3->position - triangle.v2->position, Q - triangle.v2->position), n) < -EDGE_BIAS){
		return false;
	}
	if(glm::dot(glm::cross(triangle.v1->position - triangle.v3->position, Q - triangle.v3->position), n) < -EDGE_BIAS){
		return false;
	}

	//intersection lies on triangle, successful ray hit
	intersection = Q;

	return true;
}