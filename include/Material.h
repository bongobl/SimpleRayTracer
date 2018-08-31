#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Image.h"
struct Material{

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;


	float surfaceTextureStrength;
	float envReflectionStrength;
	float refractiveIndex;
	float reflectToRefractLerp;
	Image surfaceTexture;

	Material();

};