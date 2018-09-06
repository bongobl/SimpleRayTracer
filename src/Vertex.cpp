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

bool Vertex::operator<(const Vertex& other) const{

	//compare based on position
	if(position.x != other.position.x){
		return position.x < other.position.x;
	}else if(position.y != other.position.y){
		return position.y < other.position.y;
	}else if(position.z != other.position.z){
		return position.z < other.position.z;
	}

	//texCoord
	if(texCoord.x != other.texCoord.x){
		return texCoord.x < other.texCoord.x;
	}else if(texCoord.y != other.texCoord.y){
		return texCoord.y < other.texCoord.y;
	}

	//compare based on normal
	if(normal.x != other.normal.x){
		return normal.x < other.normal.x;
	}else if(normal.y != other.normal.y){
		return normal.y < other.normal.y;
	}else if(normal.z != other.normal.z){
		return normal.z < other.normal.z;
	}
	return false;
}