#include "../include/Material.h"

Material::Material(){
	diffuse = glm::vec3(1,1,1);
	specular = glm::vec3(1,1,1);
	ambient = glm::vec3(0.15f,0.15f,0.15f);

	surfaceTextureStrength = 1.0f;
	envReflectionStrength = 1.0f;
}