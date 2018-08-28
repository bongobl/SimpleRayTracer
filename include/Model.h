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

	vector<Triangle> faces;
public:
	Material material;
	Model(string model_filename);

	friend struct Ray;
};