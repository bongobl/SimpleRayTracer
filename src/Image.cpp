#include "../include/Image.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#endif

Image::Image(){
	mode = NONE;
	
}

Image::~Image(){

}

void Image::setAsRead(std::string inFileName){
	if(mode != NONE){
		std::cerr << "Image mode already set, can't be changed" << std::endl;
		return;
	}
	mode = READ;
	//read in the file here



}
void Image::setAsWrite(int image_width, int image_height){
	if(mode != NONE){
		std::cerr << "Image mode and resolution already set, can't be changed" << std::endl;
		return;
	}
	mode = WRITE;
	width = image_width;
	height = image_height;
	pixels = new Color[height * width];
}
void Image::setPixel(int x_coord, int y_coord, float r, float g, float b){
	
	if(mode != WRITE){
		std::cerr << "Image must be in WRITE mode to set pixel" << std::endl;
		return;
	}
	//DO PARAMETER ERROR CHECKING HERE
	if(x_coord >= width || y_coord >= height || x_coord < 0 || y_coord < 0){
		return;
	}
	
	//perform function
	pixels[y_coord * height + x_coord] = Color(toColorValue(r), toColorValue(g), toColorValue(b));


}
void Image::exportPPM(std::string image_name){

	if(mode != WRITE){
		std::cerr << "Image must be in WRITE mode to exportPPM" << std::endl;
		return;
	}

	std::ofstream out(image_name.c_str());
  	out << "P3\n" << width << ' ' << height << ' ' << "255\n";

  	for (int y = 0; y < height; ++y) {
    	for (int x = 0; x < width; ++x) {
    		Color currPixel = pixels[y * height + x];
    		out << currPixel.red << ' ' << currPixel.green << ' ' << currPixel.blue << '\n';
    	}

    }
    out.close();
}

int Image::getMode(){
	return mode;
}

Vec3 Image::sample(float u, float v){


	//error checking
	if(mode == NONE){
		std::cerr << "Image must be initialized as READ or WRITE before it can be used" << std::endl;
	}
	if(u < 0 || u > 1 || v < 0 || v > 1){
		std::cerr << "UV coords must be in range 0 - 1 inclusive" << std::endl;
	}

	int x_coord = (int)(u * width);
	int y_coord = (int)((1 - v) * height);

	
	//DO PARAMETER ERROR CHECKING HERE
	if(x_coord >= width || y_coord >= height || x_coord < 0 || y_coord < 0){
		return Vec3(0,0,0);
	}

	Color pixel = pixels[y_coord * height + x_coord];
	return Vec3(toSampleValue(pixel.red), toSampleValue(pixel.green), toSampleValue(pixel.blue));

}
//private helpers
int Image::toColorValue(float value){
	float colorValue = value * 255.0f;
	if(colorValue > 255)	
		return 255;
    else if(colorValue < 0)
    	return 0;
    return (int)colorValue;
}

float Image::toSampleValue(int colorValue){
	float sampleValue = colorValue / 255.0f;
	if(sampleValue > 1.0f){
		return 1.0f;
	}else if(sampleValue < 0.0f){
		return 0.0f;
	}
	return sampleValue;
}