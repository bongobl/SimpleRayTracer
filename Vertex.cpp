#include "Vertex.h"

Vertex::Vertex() : position(Vec3()), normal(Vec3(0,0,1)) {
	
}
Vertex::~Vertex(){
	
}
Vertex::Vertex(Vec3 pos, Vec3 norm) : position(pos), normal(Vec3::normalize(norm)){

}
Vertex::Vertex(const Vertex& other) : position(other.position), normal(other.normal) {

}

void Vertex::setNormal(const Vec3& n){
	normal = Vec3::normalize(n);
}

Vec3 Vertex::getNormal() const{
	return normal;
}
Vertex& Vertex::operator=(const Vertex& other){
	position = other.position;
	normal = other.normal;
	return *this;
}