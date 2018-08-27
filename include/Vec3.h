#pragma once
#include <iostream>
using namespace std;

struct Vec3{
	
	float x,y,z;

	Vec3();
	Vec3(float x_, float y_, float z_);
	Vec3(const Vec3& other);
	~Vec3();

	//members
	float magnitude() const;
	Vec3& operator=(const Vec3& other);
	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(const Vec3& other) const;
	Vec3 operator*(const float& factor) const;
	Vec3 operator/(const float& factor) const;
	
	//statics
	static Vec3 cross(const Vec3& a, const Vec3& b);
	static float dot(const Vec3& a, const Vec3& b);
	static Vec3 normalize(const Vec3& original);
	static Vec3 reflect(const Vec3& incident, const Vec3& normal);
	
	//friends
	friend Vec3 operator*(const float& factor, const Vec3& op);
	friend ostream& operator<<(ostream& os, const Vec3& vec);
};