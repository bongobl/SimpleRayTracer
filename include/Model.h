#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vertex.h"
#include "Triangle.h"
#include "Material.h"

using namespace std;
class Model{

	//Transform (not used yet)
	glm::vec3 position;
	glm::mat4 orientation;
	glm::vec3 scale;

	//Mesh
	vector<Triangle> faces;

public:
	Material material;
	Model(string model_filename);

	friend struct Ray;
};