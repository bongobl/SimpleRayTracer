#define 	PI 		3.14159265358f
#include <iostream>
#include <thread>
#include <vector>
#include "Vec3.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Ray.h"
#include "Model.h"
#include "cmath"
#include "Image.h"
using namespace std;


const int Width = 1000;
const int Height = 1000;
const int NUM_THREADS = 60;

void renderRow(int yVal, Model& model, Image& image);
void threadTask(int startRow, Model& model, Image& image);

int main(void){
  	
	
  	Model model("ObjModels/Cube.obj");
  	Image outputFile(Width, Height);

  	cout << "Rendering Image" << endl;
  	std::vector<std::thread> threads;
  	for(unsigned int i = 0; i < NUM_THREADS; ++i){
  		threads.push_back(std::thread(threadTask, i, std::ref(model), std::ref(outputFile)));
  	}
  	for(unsigned int i = 0; i < NUM_THREADS; ++i){
  		threads[i].join();
  	}

  	outputFile.exportPPM("out.ppm");
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
		Ray ray(Vec3(-40,0, 600), Vec3( x - 500, (Height - yVal) - 500, -1000));				//perspective

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

			float red = specularAdder + ambientAdder;
			float green = diffuseFactor + specularAdder + ambientAdder;
			float blue = specularAdder + ambientAdder;

			image.setPixel(x,yVal, red, green, blue);
  		}else{

  			image.setPixel(x,yVal ,1,1,1);
  		}
	}
}