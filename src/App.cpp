#include "../include/App.h"

App::App() : simpleModel("resources/ObjModels/cube.obj"){
	
}

void App::init() {

	//create surface texture
	rockTexture.setAsRead("resources/textures/rock.jpg");

	//create material
	Material basicMaterial;
	basicMaterial.specular = glm::vec3(6, 6, 6);
	basicMaterial.surfaceTexture = rockTexture;

	//assign material to model
	simpleModel.material = basicMaterial;

	//create output image texture
	outputImage.setAsWrite(OUTPUT_WIDTH, OUTPUT_HEIGHT);
}

void App::dispose() {
	rockTexture.dispose();
	outputImage.dispose();
}
void App::renderToImage(){
	
	cout << "Rendering Image" << endl;
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(std::thread(&App::threadTask, this, i));
	}
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threads[i].join();
	}
}

void App::exportImage(std::string outputFileName) {

	cout << "Exporting Image" << endl;
	outputImage.exportPNG(outputFileName);
}
void App::threadTask(int startRow){
	for (int currRow = startRow; currRow < OUTPUT_HEIGHT; currRow += NUM_THREADS) {
		renderRow(currRow);
	}
}
void App::renderRow(int yVal){
	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;

	for (int x = 0; x < OUTPUT_WIDTH; ++x) {

		//Orthographic Rays
		//Ray ray(glm::vec3( 0.5f * (x - 520),0.5f * ((OUTPUT_HEIGHT - yVal) - 500),5000), glm::vec3(0,0,-1));		

		//Perspective Rays
		Ray ray(glm::vec3(0, 0, 0), glm::vec3(x - 500.0f, (OUTPUT_HEIGHT - yVal) - 500.0f, -1200.0f));

		//Move Camera
		glm::mat4 cameraMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-40, 0, 600));
		ray.origin = cameraMatrix * glm::vec4(ray.origin,1);
		ray.direction = cameraMatrix * glm::vec4(ray.direction, 0);

		//fire ray into scene to find color for pixel
		if (ray.intersectModel(simpleModel, fragPosition, fragTexCoord, fragNormal)) {

			//define light
			glm::vec3 directionalLight = glm::normalize(glm::vec3(1, -1, -1));

			//sample surface texture
			glm::vec3 surfaceTexture = simpleModel.material.surfaceTexture.sample(fragTexCoord);

			//calculate diffuse
			glm::vec3 diffuseComponent = glm::dot(fragNormal, -1.0f * directionalLight) * simpleModel.material.diffuse;

			//calculate specular
			glm::vec3 reflection = glm::reflect(directionalLight, fragNormal);
			glm::vec3 toEye = glm::normalize(ray.origin - fragPosition);
			glm::vec3 specularComponent = (float)pow(max(glm::dot(reflection, toEye), 0.0f), 20) * simpleModel.material.specular;

			//calculate ambient
			glm::vec3 ambientComponent = simpleModel.material.ambient;

			//combine components			
			glm::vec3 finalColor = diffuseComponent + specularComponent + ambientComponent;

			//set image pixel value
			outputImage.setPixel(x, yVal, finalColor * surfaceTexture);

		}
		else {

			//no ray intersection, set to background color
			outputImage.setPixel(x, yVal, glm::vec3(1, 1, 1));
		}
	}
}	