#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/App.h"

using namespace std;

int main(void){

  	clock_t startTime = clock();

	string outputFileName = "RenderedImage.png";
	App rayTracingApp;
	rayTracingApp.init();
	rayTracingApp.renderToImage();
	rayTracingApp.exportImage(outputFileName);
	rayTracingApp.dispose();

	clock_t endTime = clock();

	//total time of render
	double totalTime = ((double)endTime - (double)startTime) / 1000;

	//report render stats
	cout << "\nFinished rendering " << outputFileName << "\nTook " << totalTime << " seconds with " << NUM_THREADS << " threads" << endl; 

	//open file immediately after rendering
	system(outputFileName.c_str());

	return EXIT_SUCCESS;
}