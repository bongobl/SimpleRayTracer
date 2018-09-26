#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light{

	glm::vec3 direction;

public:
	Light();
	Light(glm::vec3 dir);

	void setDirection(const glm::vec3 dir);
	glm::vec3 getDirection() const;
};