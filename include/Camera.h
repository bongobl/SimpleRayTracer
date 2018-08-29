#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/Ray.h"

struct Camera{
	
	static glm::vec3 position;
	static glm::mat4 orientation;

	static float fieldOfView;
	static Ray getRay(int pixelXCoord, int pixelYCoord);
};