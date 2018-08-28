#include "../include/Vertex.h"



Vertex::Vertex() : position(glm::vec3()), texCoord(glm::vec2(0,0)), normal(glm::vec3(0,0,1)) {
	
}
Vertex::~Vertex(){
	
}
Vertex::Vertex(glm::vec3 pos, glm::vec2 uvTexCoord, glm::vec3 norm) : position(pos), texCoord(uvTexCoord), normal(glm::normalize(norm)){

}
Vertex::Vertex(const Vertex& other) : position(other.position), texCoord(other.texCoord), normal(other.normal) {

}

void Vertex::setNormal(const glm::vec3& n){
	normal = glm::normalize(n);
}

glm::vec3 Vertex::getNormal() const{
	return normal;
}
Vertex& Vertex::operator=(const Vertex& other){
	position = other.position;
	normal = other.normal;
	texCoord = other.texCoord;
	return *this;
}