#define 	PI 		3.14159265358f
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include "../include/Vec3.h"
#include "../include/Vertex.h"
#include "../include/Triangle.h"
#include "../include/Ray.h"
#include "../include/Model.h"
#include "../include/Image.h"

using namespace std;


const int Width = 1000;
const int Height = 1000;
const int NUM_THREADS = 60;

void threadTask(int startRow, Model& model, Image& image);
void renderRow(int yVal, Model& model, Image& image);


int main(void){
  	
	
	
  	Model model("ObjModels/Cube.obj");
  	Image outputFile;
  	outputFile.setAsWrite(Width, Height);

  	cout << "Rendering Image" << endl;
  	std::vector<std::thread> threads;
  	for(unsigned int i = 0; i < NUM_THREADS; ++i){
  		threads.push_back(std::thread(threadTask, i, std::ref(model), std::ref(outputFile)));
  	}
  	for(unsigned int i = 0; i < NUM_THREADS; ++i){
  		threads[i].join();
  	}

  	outputFile.exportPNG("RenderedImage.png");
  	
	return 0;
}

void threadTask(int startRow, Model& model, Image& image){
	for(int currRow = startRow; currRow < Height; currRow += NUM_THREADS){
		renderRow(currRow, model, image);
	}
}

void renderRow(int yVal, Model& model, Image& image){

	Vec3 fragPosition;
  	Vec3 fragNormal;

	for (int x = 0; x < Width; ++x) {
		//Ray ray(Vec3( 0.5f * (x - 520),0.5f * ((Height - yVal) - 500),5000), Vec3(0,0,-1));		//orthographic
		Ray ray(Vec3(-40.0f,0.0f, 600.0f), Vec3( x - 500.0f, (Height - yVal) - 500.0f, -1000.0f));				//perspective

		if(ray.intersectModel(model, fragPosition, fragNormal)){
			
			//define light
			Vec3 directionalLight = Vec3::normalize(Vec3(1,-1,-1));

			//calculate diffuse
			float diffuseFactor = Vec3::dot(fragNormal, -1 * directionalLight);

			//calculate specular
			Vec3 reflection = Vec3::reflect(directionalLight, fragNormal);
			Vec3 toEye = Vec3::normalize(ray.origin - fragPosition);
			float specularAdder = pow( max( Vec3::dot(reflection, toEye), 0.0f) , 20);

			//calculate ambient
			float ambientAdder = 0.01f;

			//combine components
			float red = specularAdder + ambientAdder;
			float green = specularAdder + ambientAdder;
			float blue = diffuseFactor + specularAdder + ambientAdder;

			//set image pixel value
			image.setPixel(x,yVal, red, green, blue);
  		}else{

  			//no ray intersection, set to background color
  			image.setPixel(x,yVal ,1,1,1);
  		}
	}
}