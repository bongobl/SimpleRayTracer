#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Triangle.h"
#include "Model.h"
struct Ray{

	glm::vec3 origin;

	glm::vec3 direction;	//ray's direction doesn't need to be normalized since its lendth doesn't matter
public:

	Ray();
	Ray(glm::vec3 origin_, glm::vec3 direction_);
	~Ray();

	bool intersectMesh(const std::vector<Model*> &allModels, glm::vec3& fragPosition, glm::vec2& fragTexCoord, glm::vec3& fragNormal, Model* &modelPtr) const;
	bool intersectTriangle(const Triangle& triangle, glm::vec3& intersection) const;
};