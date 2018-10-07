#include "../include/Camera.h"
#include "../include/App.h"

//init static camera fields
glm::vec3 Camera::position = glm::vec3(0, 0, 800);
glm::mat4 Camera::orientation = glm::inverse(glm::lookAt(glm::vec3(0, 0, 0), -1.0f * position, glm::vec3(0, 1, 0)));
int Camera::projectionMode = PERSPECTIVE;
float Camera::fieldOfView = glm::radians(30.0f);


Ray Camera::getRay(int pixelXCoord, int pixelYCoord) {

	//screen width and height independent of output resolution
	float screenWidth = 2 * 1000 * glm::tan(fieldOfView);
	float screenHeight = screenWidth * ((float)OUTPUT_HEIGHT / OUTPUT_WIDTH);

	
	Ray ray;

	if(projectionMode == ORTHO){
	
		ray.origin.x = ((float)pixelXCoord / OUTPUT_WIDTH) * screenWidth - (screenWidth / 2);
		ray.origin.y = ((float)(OUTPUT_HEIGHT - pixelYCoord) / OUTPUT_HEIGHT) * screenHeight - (screenHeight / 2);
		ray.origin.z = 5000;	//push camera far back
	}else{

		ray.direction.z = -1000;
		ray.direction.x = ((float)pixelXCoord / OUTPUT_WIDTH) * screenWidth - (screenWidth / 2);
		ray.direction.y = ((float)(OUTPUT_HEIGHT - pixelYCoord) / OUTPUT_HEIGHT) * screenHeight - (screenHeight / 2);
	}
	
	//Transform ray based on camera matrix
	glm::mat4 cameraMatrix = glm::translate(glm::mat4(1.0f),position) * orientation;
	ray.origin = cameraMatrix * glm::vec4(ray.origin, 1);
	ray.direction = cameraMatrix * glm::vec4(ray.direction, 0);

	return ray;
}
