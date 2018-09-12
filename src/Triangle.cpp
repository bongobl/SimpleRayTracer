#include "../include/Triangle.h"

Triangle::Triangle(Vertex* v1_, Vertex* v2_, Vertex* v3_, Model* _model){
	v1 = v1_;
	v2 = v2_;
	v3 = v3_;
	model = _model;
}
Triangle::~Triangle(){

}
glm::vec3 Triangle::getFaceNormal() const{
	return glm::normalize(glm::cross(v2->position - v1->position, v3->position - v1->position));
}

glm::vec3 Triangle::interpNormal(glm::vec3 sample) const{
	
	glm::vec3 faceNormal = getFaceNormal();
	float denominator = glm::dot(faceNormal, glm::cross(v2->position - v1->position, v3->position - v1->position));


	float weightV1 = glm::dot(faceNormal, glm::cross(v3->position - v2->position, sample - v2->position)) / denominator;
	float weightV2 = glm::dot(faceNormal, glm::cross(v1->position - v3->position, sample - v3->position)) / denominator;
	float weightV3 = glm::dot(faceNormal, glm::cross(v2->position - v1->position, sample - v1->position)) / denominator;

	return glm::normalize(weightV1 * v1->getNormal() + weightV2 * v2->getNormal() + weightV3 * v3->getNormal());
}


glm::vec2 Triangle::interpTexCoord(glm::vec3 sample) const {

	glm::vec3 faceNormal = getFaceNormal();
	float denominator = glm::dot(faceNormal, glm::cross(v2->position - v1->position, v3->position - v1->position));


	float weightV1 = glm::dot(faceNormal, glm::cross(v3->position - v2->position, sample - v2->position)) / denominator;
	float weightV2 = glm::dot(faceNormal, glm::cross(v1->position - v3->position, sample - v3->position)) / denominator;
	float weightV3 = glm::dot(faceNormal, glm::cross(v2->position - v1->position, sample - v1->position)) / denominator;

	return weightV1 * v1->texCoord + weightV2 * v2->texCoord + weightV3 * v3->texCoord;
}

Model* Triangle::getModel() const {
	return model;
}