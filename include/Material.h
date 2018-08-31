#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Image.h"
struct Material{

	//overall material's color
	glm::vec3 color;

	//phong reflection
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;


	float surfaceTextureStrength;
	float envReflectionStrength;
	float refractiveIndex;
	float reflectToRefractParam;
	Image surfaceTexture;

	Material();

};