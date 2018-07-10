#pragma once
#include "Vec3.h"
struct Vertex{

	Vec3 position;
private:
	Vec3 normal;	//must keep normal private to make sure it is of unit length
public: 
	
	Vertex();
	Vertex(Vec3 pos, Vec3 norm);
	Vertex(const Vertex& other);
	~Vertex();

	void setNormal(const Vec3& n);
	Vec3 getNormal() const;

	Vertex& operator=(const Vertex& other);

};