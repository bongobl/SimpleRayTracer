#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Image.h"
struct Material{

	//overall material's color
	glm::vec3 color;

	//internal color
	glm::vec3 internalColor;

	//phong reflection
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;

	//surface texture
	Image surfaceTexture;
	float surfaceTextureStrength;

	//Reflect New Rays
	bool reflectNewRay;

	//Refract New Rays
	bool refractNewRay;
	float refractiveIndex;
	float totalOpaqueDistance;
	
	//Blend all reflect/refract
	float reflectToRefractParam;

	Material();

};