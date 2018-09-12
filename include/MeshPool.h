#pragma once
#include <vector>
#include "Triangle.h"
using namespace std;

class Model;
class MeshPool{

public:
	std::vector<Model*> allModels;
	std::vector<Triangle> allFaces;

	void addModel(Model* model);

	friend struct Ray;
};