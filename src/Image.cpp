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
	width = height = 0;
	pixels = NULL;
	
}

Image::~Image(){
	
}

void Image::dispose() {
	if (pixels != NULL) {
		delete[] pixels;
		pixels = NULL;
	}
	mode = NONE;
	width = height = 0;
}
void Image::setAsRead(std::string inFileName){

	if(mode != NONE){
		std::cerr << "Image: Image mode already set, can't be changed" << std::endl;
		return;
	}
	mode = READ;

	//read in the file here
	int numChannels = -1;
	unsigned char* data;
	data = stbi_load(inFileName.c_str(), &width, &height, &numChannels, STBI_rgb);
	if (numChannels == -1) {
		std::cerr << "Image: Image name " << inFileName << " not found" << std::endl;
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
		std::cerr << "Image: Image mode and resolution already set, can't be changed" << std::endl;
		return;
	}
	mode = WRITE;
	width = image_width;
	height = image_height;
	pixels = new Color[height * width];
}

void Image::setSolidColor(glm::vec3 color) {
	if (mode != WRITE) {
		std::cerr << "Image: Image must be in WRITE mode to set Solid Color" << std::endl;
		return;
	}
	for (int pixelY = 0; pixelY < height; ++pixelY) {
		for (int pixelX = 0; pixelX < width; ++pixelX) {
			setPixel(pixelX, pixelY, color);
		}
	}
}

void Image::setPixel(int x_coord, int y_coord, glm::vec3 color){
	
	if(mode != WRITE){
		std::cerr << "Image: Image must be in WRITE mode to set pixel" << std::endl;
		return;
	}
	//DO PARAMETER ERROR CHECKING HERE
	if(x_coord >= width || y_coord >= height || x_coord < 0 || y_coord < 0){
		return;
	}
	
	//perform function
	pixels[y_coord * width + x_coord] = Color(toColorValue(color.r), toColorValue(color.g), toColorValue(color.b));


}
void Image::exportPNG(std::string image_name) const{

	//error check
	if(mode != WRITE){
		std::cerr << "Image: Image must be in WRITE mode to exportPPM" << std::endl;
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

int Image::getMode() const{
	return mode;
}

glm::vec3 Image::sample(glm::vec2 param) const{


	//return plain white color if image pixels don't exist
	if(mode == NONE){
		return glm::vec3(1,1,1);
	}

	
	//convert to pixel coordinates
	int x_coord = (int)(param.x * width);
	int y_coord = (int)((1 - param.y) * height);

	
	//bounds checking (can't use setPixel if in read mode)
	if (x_coord >= width)
		x_coord = width - 1;
	if (y_coord >= height)
		y_coord = height - 1;
	if (x_coord < 0)
		x_coord = 0;
	if (y_coord < 0)
		y_coord = 0;
	

	//sample image and return color
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