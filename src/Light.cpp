#include "../include/Light.h"

Light::Light(){
	direction = glm::vec3(0,0,1);
}
Light::Light(const glm::vec3 dir){
	if(dir == glm::vec3(0,0,0)){
		direction = glm::vec3(0,0,1);
	}else{
		direction = glm::normalize(dir);
	}
}

void Light::setDirection(const glm::vec3 dir){
	if(dir == glm::vec3(0,0,0)){
		direction = glm::vec3(0,0,1);
	}else{
		direction = glm::normalize(dir);
	}
}

glm::vec3 Light::getDirection() const{
	return direction;
}