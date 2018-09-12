#include "../include/RenderUtils.h"
#include "../include/Model.h"
#include "../include/Ray.h"
#include "../include/CubeMap.h"


glm::vec3 RenderUtils::refract(glm::vec3 incident, glm::vec3 normal, float nVacuum, float nMaterial){
	
	//make sure incident angle is normalized
	incident = glm::normalize(incident);

	//calc incident angle
	float incidentAngle = glm::acos(glm::dot(-1.0f * incident, normal));

	float n1;
	float n2;

	//Use incident angle to determine if ray is coming from inside material, or from vacuum
	if (incidentAngle > glm::radians(90.0f)) {
		incidentAngle = glm::radians(180.0f) - incidentAngle;
		normal = -1.0f * normal;
		n1 = nMaterial;
		n2 = nVacuum;
	}
	else {
		n1 = nVacuum;
		n2 = nMaterial;
	}

	//check for total internal reflection if ray is incident inside material
	if (n1 > n2) {

		//find criticalAngle
		float criticalAngle = glm::asin(n2 / n1);

		//if incident angle >= critical angle, ray gets reflected back in material
		if (incidentAngle >= criticalAngle) {
			return glm::reflect(incident, normal);
		}
	}

	//calc reflective angle using Snell's law
	float refractAngle = glm::asin((n1 / n2) * glm::sin(incidentAngle));

	//calc pivot vector
	glm::vec3 pivot = glm::cross(incident, normal);

	//rotate negative normal around pivot by refractive angle amount to get final angle
	return glm::rotate(glm::mat4(1.0f), refractAngle, pivot) * glm::vec4(-1.0f * normal, 0);
}