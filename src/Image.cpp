#include "../include/Image.h"
#include <vector>

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
	int numChannels = -1;
	unsigned char* data;
	data = stbi_load(inFileName.c_str(), &width, &height, &numChannels, STBI_rgb);
	if (numChannels == -1) {
		std::cerr << "Image name " << inFileName << " not found" << std::endl;
		return;
	}
	pixels = new Color[height * width];
	
	//parse here
	for(int i = 0; i < width * height; ++i){ 
		Color* currPixel = &pixels[i];

		currPixel->red = data[i * 3 + 0];
		currPixel->green = data[i * 3 + 1];
		currPixel->blue = data[i * 3 + 2];

	}
	stbi_image_free(data);

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
void Image::setPixel(int x_coord, int y_coord, glm::vec3 color){
	
	if(mode != WRITE){
		std::cerr << "Image must be in WRITE mode to set pixel" << std::endl;
		return;
	}
	//DO PARAMETER ERROR CHECKING HERE
	if(x_coord >= width || y_coord >= height || x_coord < 0 || y_coord < 0){
		return;
	}
	
	//perform function
	pixels[y_coord * width + x_coord] = Color(toColorValue(color.r), toColorValue(color.g), toColorValue(color.b));


}
void Image::exportPNG(std::string image_name){

	//error check
	if(mode != WRITE){
		std::cerr << "Image must be in WRITE mode to exportPPM" << std::endl;
		return;
	}

	//export png
	std::vector<unsigned char> data;
    for(int i = 0; i < width * height; ++i){ 
		Color currPixel = pixels[i];
		data.push_back((unsigned char)currPixel.red);
		data.push_back((unsigned char)currPixel.green);
		data.push_back((unsigned char)currPixel.blue);
	}
    stbi_write_png(image_name.c_str(), width, height, 3, data.data(), width * 3);
}

int Image::getMode(){
	return mode;
}

glm::vec3 Image::sample(glm::vec2 param) const{


	//error checking
	if(mode == NONE){
		std::cerr << "Image must be initialized as READ or WRITE before it can be used" << std::endl;
	}
	if(param.x < 0 || param.x > 1 || param.y < 0 || param.y > 1){
		std::cerr << "UV coords must be in range 0 - 1 inclusive" << std::endl;
	}

	int x_coord = (int)(param.x * width);
	int y_coord = (int)((1 - param.y) * height);

	
	//DO PARAMETER ERROR CHECKING HERE
	if(x_coord >= width || y_coord >= height || x_coord < 0 || y_coord < 0){
		return glm::vec3(0,0,0);
	}

	Color pixel = pixels[y_coord * width + x_coord];
	return glm::vec3(toSampleValue(pixel.red), toSampleValue(pixel.green), toSampleValue(pixel.blue));

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