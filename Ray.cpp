#define EDGE_BIAS 0.006	//when hitpoint on edge or point
#include "Ray.h"

Ray::Ray(Vec3 origin_, Vec3 direction_) : origin(origin_), direction(direction_) {

}
Ray::~Ray(){

}

bool Ray::intersectModel(const Model& model, Vec3& fragPosition, Vec3& fragNormal) const{

	//was a face hit
	bool hitFace = false;

	//distance to running closest face
	float minDist;

	//loop through all faces in model
	for(unsigned int currFace = 0; currFace < model.faces.size(); ++currFace){

		//used to find intersection position if ray hits polygon
		Vec3 currIntersection;

		//if ray hits triangle
		if(intersectTriangle(model.faces.at(currFace), currIntersection)){

			//debug
			//cout << "hit a triangle" << endl;

			float currDist = (currIntersection - origin).magnitude();

			//if first time we hit a face, OR we hit a new closest face
			if(!hitFace || currDist < minDist){
				fragPosition = currIntersection;
				fragNormal = model.faces.at(currFace).interpNormal(currIntersection);
				minDist = currDist;
			}

			hitFace = true;
		}
	}

	//return whether or not a face was hit
	return hitFace;
}


bool Ray::intersectTriangle(const Triangle& triangle, Vec3& intersection) const{

	//if ray is perpendicular to triangle normal, they don't intersect
	if(Vec3::dot(triangle.getFaceNormal(), direction) >= 0){
		return false;
	}
	
	Vec3 n = triangle.getFaceNormal();
	Vec3 dir = Vec3::normalize(direction);
	float d = Vec3::dot(n, triangle.v1.position);

	float t = (d - Vec3::dot(n, origin)) / Vec3::dot(n,dir);

	//if t is negative, ray is going away from triangle
	if(t < 0){
		return false;
	}

	//calclate intersection position
	Vec3 Q = origin + t * dir;

	//run through conclusive cases of intersection point out of triangle
	if(Vec3::dot(Vec3::cross(triangle.v2.position - triangle.v1.position, Q - triangle.v1.position), n) < -EDGE_BIAS){
		//cout << "a" << endl;
		return false;
	}
	if(Vec3::dot(Vec3::cross(triangle.v3.position - triangle.v2.position, Q - triangle.v2.position), n) < -EDGE_BIAS){
		//cout << "b" << endl;
		return false;
	}
	if(Vec3::dot(Vec3::cross(triangle.v1.position - triangle.v3.position, Q - triangle.v3.position), n) < -EDGE_BIAS){
		//cout << "c" << endl;
		return false;
	}

	//intersection lies on triangle, successful ray hit
	intersection = Q;
	return true;
}