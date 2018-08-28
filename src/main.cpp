#define 	PI 		3.14159265358f
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/App.h"

using namespace std;

int main(void){
  	

	App rayTracingApp;
	rayTracingApp.init();
	rayTracingApp.renderToImage();
	rayTracingApp.exportImage("RenderedImage.png");
	rayTracingApp.dispose();

	return EXIT_SUCCESS;
}