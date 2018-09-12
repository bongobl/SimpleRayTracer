#include "../include/App.h"

App::App() : simpleModel("resources/ObjModels/FilletCube.obj"){
	
}

void App::init() {

	//create environment map
	environmentMap.rightFace.setAsRead("resources/textures/CubeMap/right.jpg");
	environmentMap.leftFace.setAsRead("resources/textures/CubeMap/left.jpg");
	environmentMap.topFace.setAsRead("resources/textures/CubeMap/top.jpg");
	environmentMap.bottomFace.setAsRead("resources/textures/CubeMap/bottom.jpg");
	environmentMap.frontFace.setAsRead("resources/textures/CubeMap/front.jpg");
	environmentMap.backFace.setAsRead("resources/textures/CubeMap/back.jpg");

	//create surface texture
	rockTexture.setAsRead("resources/textures/rock.jpg");

	//create material
	Material basicMaterial;
	basicMaterial.specular = glm::vec3(1, 1, 1);
	basicMaterial.surfaceTexture = rockTexture;
	basicMaterial.surfaceTextureStrength = 0.0f;
	basicMaterial.refractiveIndex = 1.03f;
	basicMaterial.reflectToRefractParam = 0.0f;


	//assign material to model
	simpleModel.material = basicMaterial;

	allModels.push_back(&simpleModel);

	//create output image texture
	outputImage.setAsWrite(OUTPUT_WIDTH, OUTPUT_HEIGHT);

	//transform model
	simpleModel.setOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(55.0f), glm::vec3(2, 4, -1)));
	simpleModel.setPosition(glm::vec3(-250, -15, 150));


}

void App::dispose() {

	//clean up CubeMap
	environmentMap.rightFace.dispose();
	environmentMap.leftFace.dispose();
	environmentMap.topFace.dispose();
	environmentMap.bottomFace.dispose();
	environmentMap.frontFace.dispose();
	environmentMap.backFace.dispose();

	//clean up surface texture
	rockTexture.dispose();

	//clean up output image
	outputImage.dispose();
}
void App::renderToImage(){
	
	cout << "Rendering Image" << endl;
	renderMesh();	
}

void App::exportImage(std::string outputFileName) {

	cout << "Exporting Image " << outputFileName << " to disk" << endl;
	outputImage.exportPNG(outputFileName);
}


//Private Helpers
void App::renderMesh() {
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(std::thread(&App::threadTask, this, i));
	}
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threads[i].join();
	}
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

	for (unsigned int currModel = 0; currModel < allModels.size(); ++currModel) {

		for (int x = 0; x < OUTPUT_WIDTH; ++x) {

			calcPixelColor(*allModels[currModel], x, yVal);

		}//For Each Pixel

	}//For each Model

}//End render row	

void App::calcPixelColor(const Model& model, int pixelX, int pixelY) {
	//Perform lighting calculations for this particular pixel

	//interpolated vertex attributes
	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;

	//get ray
	Ray ray = Camera::getRay(pixelX, pixelY);

	//fire ray into scene to find color for pixel
	if (ray.intersectModel(model, fragPosition, fragTexCoord, fragNormal)) {

		//define light
		glm::vec3 directionalLight = glm::normalize(glm::vec3(-1, -1, -1));

		//get current model material
		Material currMaterial = model.material;

		//sample surface texture
		glm::vec3 surfaceTextureColor = glm::vec3(1, 1, 1) - (currMaterial.surfaceTextureStrength *
			(glm::vec3(1, 1, 1) - currMaterial.surfaceTexture.sample(fragTexCoord)));

		//sample environment map
		glm::vec3 cubeMapSampler = glm::reflect(ray.direction, fragNormal);
		glm::vec3 envReflectionColor = glm::vec3(1, 1, 1) - (currMaterial.envReflectionStrength *
			(glm::vec3(1, 1, 1) - environmentMap.sample(cubeMapSampler)));

		//calculate refraction
		glm::vec3 refractionColor = refractedColor(model, ray);

		//calculate diffuse
		glm::vec3 diffuseComponent = max(glm::dot(fragNormal, -1.0f * directionalLight), 0.0f) * currMaterial.diffuse;

		//calculate specular
		glm::vec3 reflection = glm::reflect(directionalLight, fragNormal);
		glm::vec3 toEye = glm::normalize(ray.origin - fragPosition);
		glm::vec3 specularComponent = (float)pow(max(glm::dot(reflection, toEye), 0.0f), 20) * currMaterial.specular;

		//calculate ambient
		glm::vec3 ambientComponent = currMaterial.ambient;

		//combine reflection components			
		glm::vec3 totalReflectionColor = envReflectionColor * surfaceTextureColor * (diffuseComponent + specularComponent + ambientComponent);


		//define refLeft based on param and normal's relation to camera
		float refLerp;
		float normalDotFragToCam = glm::pow(glm::max(glm::dot(glm::normalize(ray.origin - fragPosition), fragNormal), 0.0f), 0.7f);

		if (currMaterial.reflectToRefractParam <= 0.5f) {
			refLerp = 2 * currMaterial.reflectToRefractParam * normalDotFragToCam;
		}
		else {
			refLerp = 1 - 2 * (1 - currMaterial.reflectToRefractParam) * (1 - normalDotFragToCam);
		}
		//lerp between reflectivness and refractivness
		glm::vec3 finalColor = currMaterial.color * ((1 - refLerp) * totalReflectionColor + (refLerp)* refractionColor);

		//set image pixel value
		outputImage.setPixel(pixelX, pixelY, finalColor);

	}
	else {

		//don't use when rendering multiple objects, this will draw over previously rendered objects
		outputImage.setPixel(pixelX, pixelY, environmentMap.sample(ray.direction));
	}
}
glm::vec3 App::refractedColor(const Model& model, Ray ray) {

	Ray finalRay = ray;

	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;

	float matRefIndex = model.material.refractiveIndex;

	//go in model
	if (finalRay.intersectModel(model, fragPosition, fragTexCoord, fragNormal)) {
		finalRay.origin = fragPosition;
		finalRay.direction = refract(finalRay.direction, fragNormal, 1, matRefIndex);
	}
	//go out model
	if (finalRay.intersectModel(model, fragPosition, fragTexCoord, fragNormal)) {
		finalRay.origin = fragPosition;
		finalRay.direction = refract(finalRay.direction, fragNormal, 1, matRefIndex);
	}

	return environmentMap.sample(finalRay.direction);
}

glm::vec3 App::refract(glm::vec3 incident, glm::vec3 normal, float nVacuum, float nMaterial){

	//make sure incident angle is normalized
	incident = glm::normalize(incident);
	
	//calc incident angle
	float incidentAngle = glm::acos(glm::dot(-1.0f * incident, normal));

	float n1;	
	float n2;	

	//Use incident angle to determine if ray is coming from inside material, or from vacuum
	if(incidentAngle > glm::radians(90.0f)){
		incidentAngle = glm::radians(180.0f) - incidentAngle;
		normal = -1.0f * normal;
		n1 = nMaterial;
		n2 = nVacuum;
	}
	else {
		n1 = nVacuum;
		n2 = nMaterial;
	}

	//check for total internal reflection if ray is incident inside material
	if (n1 > n2) {

		//find criticalAngle
		float criticalAngle = glm::asin(n2 / n1);

		//if incident angle >= critical angle, ray gets reflected back in material
		if (incidentAngle >= criticalAngle) {
			return glm::reflect(incident, normal);
		}
	}

	//calc reflective angle using Snell's law
	float refractAngle = glm::asin( (n1 / n2) * glm::sin(incidentAngle) );

	//calc pivot vector
	glm::vec3 pivot = glm::cross(incident, normal);

	//rotate negative normal around pivot by refractive angle amount to get final angle
	return glm::rotate(glm::mat4(1.0f), refractAngle, pivot) * glm::vec4(-1.0f * normal,0);

}