#pragma once
#include "Vec3.h"
#include "Triangle.h"
#include "Model.h"
struct Ray{

	Vec3 origin;
private:
	Vec3 direction;	
public:

	Ray(Vec3 origin_, Vec3 direction_);
	~Ray();

	bool intersectModel(const Model& model, Vec3& fragPosition, Vec3& fragNormal) const;
	bool intersectTriangle(const Triangle& triangle, Vec3& intersection) const;
};