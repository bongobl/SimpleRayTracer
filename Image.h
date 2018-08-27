#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
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
	void exportPPM(std::string image_name);
	int getMode();

	enum IOMode {NONE, READ, WRITE};

private:
	static int toColorValue(float value);
};