#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Image.h"
struct Material{

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;

	Image surfaceTexture;

	Material();
	Material(Image surface_texture);
	Material(Image surface_texture, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _ambient);


		
};