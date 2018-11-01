#include "../include/Material.h"

Material::Material(){
	
	//overall material's color
	color = glm::vec3(1, 1, 1);

	//internal color
	internalColor = glm::vec3(1,1,1);

	//phone reflection
	diffuse = glm::vec3(1,1,1);
	specular = glm::vec3(1,1,1);
	ambient = glm::vec3(0.15f,0.15f,0.15f);

	//surface texture
	surfaceTextureStrength = 1.0f;

	//reflect new ray
	reflectNewRay = false;

	//Refract New Rays
	refractNewRay = false;
	refractiveIndex = 1.0f;
	totalOpaqueDistance = 100.0f;	//not used yet

	//Blend all reflect/refract
	reflectToRefractParam = 0.0f;
}