#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Triangle.h"
#include "Model.h"
struct Ray{

	glm::vec3 origin;
private:
	glm::vec3 direction;	
public:

	Ray(glm::vec3 origin_, glm::vec3 direction_);
	~Ray();

	bool intersectModel(const Model& model, glm::vec3& fragPosition, glm::vec2& fragTexCoord, glm::vec3& fragNormal) const;
	bool intersectTriangle(const Triangle& triangle, glm::vec3& intersection) const;
};