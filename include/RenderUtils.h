#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RenderUtils{
	
public:

	static glm::vec3 refract(glm::vec3 incident, glm::vec3 normal, float nVacuum, float nMaterial);
};