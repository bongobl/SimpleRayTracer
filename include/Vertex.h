#pragma once
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Vertex{

	glm::vec3 position;
	glm::vec2 texCoord;
private:
	glm::vec3 normal;	//must keep normal private to make sure it is of unit length
public: 
	
	Vertex();
	Vertex(glm::vec3 pos, glm::vec2 uvTexCoord, glm::vec3 norm);
	Vertex(const Vertex& other);
	~Vertex();

	void setNormal(const glm::vec3& n);
	glm::vec3 getNormal() const;

	Vertex& operator=(const Vertex& other);
	bool operator<(const Vertex& other) const;

};