#include "../include/Vec3.h"
#include <cmath>

Vec3::Vec3(){
	x = y = z = 0.0;
}

Vec3::~Vec3(){
	
}

Vec3::Vec3(float x_, float y_, float z_){
	x = x_;
	y = y_;
	z = z_;
}

Vec3::Vec3(const Vec3& other){
	x = other.x;
	y = other.y;
	z = other.z;
}

float Vec3::magnitude() const{
	return sqrt(x*x + y*y + z*z);
}

Vec3& Vec3::operator=(const Vec3& other){
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3& other) const{
	return Vec3(x + other.x, y + other.y, z + other.z);
}
Vec3 Vec3::operator-(const Vec3& other) const{
	return Vec3(x - other.x, y - other.y, z - other.z);
}
Vec3 Vec3::operator*(const Vec3& other) const{
	return Vec3(x * other.x, y * other.y, z * other.z);
}
Vec3 Vec3::operator*(const float& factor) const{
	return Vec3(factor * x, factor * y, factor * z);
}
Vec3 Vec3::operator/(const float& factor) const{
	return Vec3(x / factor, y / factor, z / factor);
}

//statics and helpers
Vec3 Vec3::cross(const Vec3& a, const Vec3& b){
	Vec3 c;
	c.x = a.y * b.z - b.y * a.z;
	c.y = b.x * a.z - a.x * b.z;
	c.z = a.x * b.y - b.x * a.y;
	return c;
}

float Vec3::dot(const Vec3& a, const Vec3& b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


Vec3 Vec3::normalize(const Vec3& original){
	float length = original.magnitude();
	return Vec3(original.x / length, original.y / length, original.z / length);
}
Vec3 Vec3::reflect(const Vec3& incident, const Vec3& normal){
	Vec3 L = -1 * incident;
	return 2 * dot(normalize(normal), L) * normalize(normal) - L;
}

//friends
Vec3 operator*(const float& factor, const Vec3& op){
	return Vec3(factor * op.x, factor * op.y, factor * op.z);
}
ostream& operator<<(ostream& os, const Vec3& vec)  
{  
    os << "Vec3:(" << vec.x << "," << vec.y << "," << vec.z << ")";
    return os;  
}  