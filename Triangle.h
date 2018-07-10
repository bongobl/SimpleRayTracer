#pragma once
#include "Vec3.h"
#include "Vertex.h"
class Triangle{
public:
	Vertex v1, v2, v3;

	Triangle(Vertex v1_, Vertex v2_, Vertex v3_);
	~Triangle();
	Vec3 getFaceNormal() const;
	
	Vec3 interpNormal(Vec3 sample) const;
};