#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/Ray.h"
#include "../include/Model.h"
#include "../include/Image.h"

const int OUTPUT_WIDTH = 1000;
const int OUTPUT_HEIGHT = 1000;
const int NUM_THREADS = 60;

class App{
	Image rockTexture;
	Model simpleModel;
	Image outputImage;

public:

	App();
	void init();
	void dispose();
	void renderToImage();
	void exportImage(std::string outputFileName);

private:
	void threadTask(int startRow);
	void renderRow(int yVal);	
};