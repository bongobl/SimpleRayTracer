#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Vertex.h"
#include "Triangle.h"
using namespace std;
class Model{

	vector<Triangle> faces;
public:
	Model(string model_filename);

	friend struct Ray;
};