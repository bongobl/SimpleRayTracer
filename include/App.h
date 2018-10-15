#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"
#include "Camera.h"
#include "Ray.h"
#include "Model.h"
#include "Image.h"
#include "CubeMap.h"
#include "Camera.h"
#include "RenderUtils.h"

const int OUTPUT_WIDTH = 1280;
const int OUTPUT_HEIGHT = 720;
const int NUM_THREADS = 90;
const int MAX_NUM_RAY_BOUNCES = 3;

class App{

	Model sphere;
	Model ico;

	Image outputImage;
	CubeMap environmentMap;

	std::vector<Model*> allModels;

	Light directionalLight;
	
public:

	App();
	void init();
	void dispose();
	void renderToImage();
	void exportImage(std::string outputFileName);


private:
	void renderMesh();
	void threadTask(int startRow);
	void renderRow(int yVal);

	glm::vec3 getColorFromScene(Ray ray, int numBounces);
};