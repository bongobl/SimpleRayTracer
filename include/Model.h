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
	glm::mat4 toWorld;
	glm::mat3 toWorld3x3;

	//Mesh
	vector<Vertex*> rawVertices;
	vector<Vertex*> vertices;
	vector<Triangle> faces;

public:

	//material
	Material material;


	Model(string model_filename);
	~Model();

	//position
	void setPosition(const glm::vec3 model_position);
	glm::vec3 getPosition() const;

	//orientation
	void setOrientation(const glm::mat4 model_orientation);
	glm::mat4 getOrientation() const;

	//scale
	void setScale(const glm::vec3 model_scale);
	glm::vec3 getScale() const;

	//toWorld
	glm::mat4 getToWorld() const;
	glm::mat3 getToWorld3x3() const;

	friend struct Ray;
	friend class MeshPool;
private:

	//update model toWorld matrices to reflect position, orientation and scale
	void updateToWorldAndMesh();
};