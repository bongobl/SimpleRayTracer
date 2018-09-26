#pragma once
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vertex.h"

using namespace std;
class Model;

class Triangle{

public:
	Vertex *v1, *v2, *v3;
	

	Triangle(Vertex* v1_, Vertex* v2_, Vertex* v3_);
	~Triangle();
	glm::vec3 getFaceNormal() const;
	
	glm::vec3 interpNormal(glm::vec3 sample) const;
	glm::vec2 interpTexCoord(glm::vec3 sample) const;
};