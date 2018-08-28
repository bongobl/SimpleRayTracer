#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//Used to store RGB values for image to be written
//Use Vec3 instead for all in-app purposes
struct Color{

	int red;
	int green;
	int blue;

	Color() {red = green = blue = 0;}
	Color(int r, int g, int b) { red = r, green = g, blue = b;}
};

class Image{
	int width;
	int height;
	Color* pixels;
	int mode;
	


public:
	Image();
	~Image();

	void setAsRead(std::string inFileName);
	void setAsWrite(int image_width, int image_height);

	void setPixel(int x_coord, int y_coord, float r, float g, float b);
	void exportPNG(std::string image_name);
	int getMode();

	glm::vec3 sample(glm::vec2 param) const;

	enum IOMode {NONE, READ, WRITE};

private:
	static int toColorValue(float value);
	static float toSampleValue(int colorValue);
};