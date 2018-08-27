#include "../include/Triangle.h"

Triangle::Triangle(Vertex v1_, Vertex v2_, Vertex v3_){
	v1 = v1_;
	v2 = v2_;
	v3 = v3_;
}
Triangle::~Triangle(){

}
Vec3 Triangle::getFaceNormal() const{
	return Vec3::normalize(Vec3::cross(v2.position - v1.position, v3.position - v1.position));
}

Vec3 Triangle::interpNormal(Vec3 sample) const{
	
	Vec3 faceNormal = getFaceNormal();
	float denominator = Vec3::dot(faceNormal, Vec3::cross(v2.position - v1.position, v3.position - v1.position));


	float weightV1 = Vec3::dot(faceNormal, Vec3::cross(v3.position - v2.position, sample - v2.position)) / denominator;
	float weightV2 = Vec3::dot(faceNormal, Vec3::cross(v1.position - v3.position, sample - v3.position)) / denominator;
	float weightV3 = Vec3::dot(faceNormal, Vec3::cross(v2.position - v1.position, sample - v1.position)) / denominator;

	return Vec3::normalize(weightV1 * v1.getNormal() + weightV2 * v2.getNormal() + weightV3 * v3.getNormal());
}