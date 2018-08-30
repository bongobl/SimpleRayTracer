#include "../include/Camera.h"
#include "../include/App.h"
glm::vec3 Camera::position = glm::vec3(400, 400, 400);
glm::mat4 Camera::orientation = glm::inverse(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, -1, -1), glm::vec3(0, 1, 0)));

float Camera::fieldOfView = glm::radians(30.0f);

Ray Camera::getRay(int pixelXCoord, int pixelYCoord) {

	//screen width and height independent of output resolution
	float screenWidth = 2 * 1000 * glm::tan(fieldOfView);
	float screenHeight = screenWidth * ((float)OUTPUT_HEIGHT / OUTPUT_WIDTH);

	//Ortho Ray (don't use 'til fixed)
	//Ray ray(glm::vec3( 0.5f * (pixelXCoord - OUTPUT_WIDTH / 2), 0.5f * ((OUTPUT_HEIGHT - pixelYCoord) - OUTPUT_HEIGHT / 2),5000), glm::vec3(0,0,-1));

	
	Ray ray;

	//Ortho Ray
	//ray.origin.x = ((float)pixelXCoord / OUTPUT_WIDTH) * screenWidth - (screenWidth / 2);
	//ray.origin.y = ((float)(OUTPUT_HEIGHT - pixelYCoord) / OUTPUT_HEIGHT) * screenHeight - (screenHeight / 2);
	//ray.origin.z = 5000;

	//Perspective Ray
	ray.direction.z = -1000;
	ray.direction.x = ((float)pixelXCoord / OUTPUT_WIDTH) * screenWidth - (screenWidth / 2);
	ray.direction.y = ((float)(OUTPUT_HEIGHT - pixelYCoord) / OUTPUT_HEIGHT) * screenHeight - (screenHeight / 2);

	//Move basid on camera matrix
	glm::mat4 cameraMatrix = glm::translate(glm::mat4(1.0f),position) * orientation;
	ray.origin = cameraMatrix * glm::vec4(ray.origin, 1);
	ray.direction = cameraMatrix * glm::vec4(ray.direction, 0);

	return ray;
}