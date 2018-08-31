#define 	PI 		3.14159265358f
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/App.h"

using namespace std;

int main(void){
  	
	string outputFileName = "RenderedImage.png";
	App rayTracingApp;
	rayTracingApp.init();
	rayTracingApp.renderToImage();
	rayTracingApp.exportImage(outputFileName);
	rayTracingApp.dispose();

	system(outputFileName.c_str());
	return EXIT_SUCCESS;
}