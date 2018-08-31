#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/Camera.h"
#include "../include/Ray.h"
#include "../include/Model.h"
#include "../include/Image.h"
#include "../include/CubeMap.h"
#include "../include/Camera.h"


const int OUTPUT_WIDTH = 1280;
const int OUTPUT_HEIGHT = 720;
const int NUM_THREADS = 90;

class App{
	Image rockTexture;
	Model simpleModel;
	Image outputImage;
	CubeMap environmentMap;

	
public:

	App();
	void init();
	void dispose();
	void renderToImage();
	void exportImage(std::string outputFileName);


private:
	void renderModel(Model& model);
	void threadTask(int startRow, Model& model);
	void renderRow(int yVal, Model& model);	
	glm::vec3 refractedColor(Model& model, Ray ray);
	glm::vec3 refract(glm::vec3 incident, glm::vec3 normal, float nVacuum, float nMaterial);
};