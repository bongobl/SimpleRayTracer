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

constexpr int OUTPUT_WIDTH = static_cast<int>(720 * 1.5f);
constexpr int OUTPUT_HEIGHT = static_cast<int>(720 * 1.5f);
constexpr int NUM_THREADS = OUTPUT_HEIGHT;
constexpr int MAX_NUM_RAY_BOUNCES = 4;

class App{

	Model cube;
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

	glm::vec3 getColorFromScene(const Ray ray);
};