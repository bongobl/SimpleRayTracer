#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Ray.h"
#include "Model.h"
#include "Image.h"
#include "CubeMap.h"
#include "Camera.h"
#include "RenderUtils.h"
#include "MeshPool.h"

const int OUTPUT_WIDTH = 1280;
const int OUTPUT_HEIGHT = 720;
const int NUM_THREADS = 90;

class App{

	Image rockTexture;
	Model simpleModel;
	Image outputImage;
	CubeMap environmentMap;

	std::vector<Model*> allModels;

	
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

	void calcPixelColor(const Model& model, int pixelX, int pixelY);
	glm::vec3 refractedColor(const Model& model, Ray ray);
};