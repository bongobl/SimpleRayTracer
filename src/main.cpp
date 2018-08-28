#define 	PI 		3.14159265358f
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

Image rockTexture;

int main(void){
  	
	rockTexture.setAsRead("resources/textures/rock.jpg");
  	Model model("resources/ObjModels/cube.obj");
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

	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
  	glm::vec3 fragNormal;

	for (int x = 0; x < Width; ++x) {
		//Ray ray(Vec3( 0.5f * (x - 520),0.5f * ((Height - yVal) - 500),5000), Vec3(0,0,-1));		//orthographic
		Ray ray(glm::vec3(-40.0f,0.0f, 500.0f), glm::vec3( x - 500.0f, (Height - yVal) - 500.0f, -1000.0f));				//perspective

		if(ray.intersectModel(model, fragPosition, fragTexCoord, fragNormal)){
			
			//define light
			glm::vec3 directionalLight = glm::normalize(glm::vec3(1,-1,-1));

			//calculate diffuse
			float diffuseFactor = glm::dot(fragNormal, -1.0f * directionalLight);

			//calculate specular
			glm::vec3 reflection = glm::reflect(directionalLight, fragNormal);
			glm::vec3 toEye = glm::normalize(ray.origin - fragPosition);
			float specularAdder = pow( max( glm::dot(reflection, toEye), 0.0f) , 20);

			//calculate ambient
			float ambientAdder = 0.01f;

			//combine components
			
			float red = diffuseFactor + specularAdder + ambientAdder;
			float green = diffuseFactor + specularAdder + ambientAdder;
			float blue = diffuseFactor + specularAdder + ambientAdder;
			//set image pixel value
			image.setPixel(x, yVal, glm::vec3(red, green, blue) * rockTexture.sample(fragTexCoord));
			

			
			//glm::vec3 finalColor = rockTexture.sample(fragTexCoord);
			//set image pixel value
			//image.setPixel(x,yVal, finalColor.x, finalColor.y, finalColor.z);
			
  		}else{

  			//no ray intersection, set to background color
  			image.setPixel(x,yVal ,glm::vec3(1,1,1));
  		}
	}
}