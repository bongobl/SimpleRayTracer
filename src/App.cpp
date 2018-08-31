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
	basicMaterial.reflectToRefractLerp = 0.9f;

	//assign material to model
	simpleModel.material = basicMaterial;

	//create output image texture
	outputImage.setAsWrite(OUTPUT_WIDTH, OUTPUT_HEIGHT);
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
	outputImage.setSolidColor(glm::vec3(0, 0.5f, 0.5f));
	renderModel(simpleModel);

	
}

void App::exportImage(std::string outputFileName) {

	cout << "Exporting Image " << outputFileName << " to disk" << endl;
	outputImage.exportPNG(outputFileName);
}


//Private Helpers
void App::renderModel(Model& model) {
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(std::thread(&App::threadTask, this, i, std::ref(model)));
	}
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threads[i].join();
	}
}
void App::threadTask(int startRow, Model& model){
	for (int currRow = startRow; currRow < OUTPUT_HEIGHT; currRow += NUM_THREADS) {
		renderRow(currRow, model);
	}
}
void App::renderRow(int yVal, Model& model){
	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;

	for (int x = 0; x < OUTPUT_WIDTH; ++x) {

		Ray ray = Camera::getRay(x, yVal);

		//fire ray into scene to find color for pixel
		if (ray.intersectModel(model, fragPosition, fragTexCoord, fragNormal)) {

			//define light
			glm::vec3 directionalLight = glm::normalize(glm::vec3(-1, -1, -1));

			//sample surface texture
			glm::vec3 surfaceTextureColor = glm::vec3(1, 1, 1) - (model.material.surfaceTextureStrength *
				(glm::vec3(1, 1, 1) - model.material.surfaceTexture.sample(fragTexCoord)));

			//sample environment map
			glm::vec3 cubeMapSampler = glm::reflect(ray.direction, fragNormal);
			glm::vec3 environmentColor = glm::vec3(1, 1, 1) - (model.material.envReflectionStrength *
				(glm::vec3(1, 1, 1) - environmentMap.sample(cubeMapSampler)));

			//calculate refraction
			glm::vec3 refractionColor = refractedColor(model, ray);

			//calculate diffuse
			glm::vec3 diffuseComponent = max(glm::dot(fragNormal, -1.0f * directionalLight),0.0f) * model.material.diffuse;

			//calculate specular
			glm::vec3 reflection = glm::reflect(directionalLight, fragNormal);
			glm::vec3 toEye = glm::normalize(ray.origin - fragPosition);
			glm::vec3 specularComponent = (float)pow(max(glm::dot(reflection, toEye), 0.0f), 20) * model.material.specular;

			//calculate ambient
			glm::vec3 ambientComponent = model.material.ambient;

			//combine reflection components			
			glm::vec3 totalReflectionColor = environmentColor * surfaceTextureColor * (diffuseComponent + specularComponent + ambientComponent);

			//lerp between reflectivness and refractivness
			float refLerp = model.material.reflectToRefractLerp;
			glm::vec3 finalColor = (1 - refLerp) * totalReflectionColor + (refLerp) * refractionColor;
	
			//set image pixel value
			outputImage.setPixel(x, yVal, finalColor);

		}
		else {

			//don't use when rendering multiple objects, this will draw over previously rendered objects
			outputImage.setPixel(x, yVal, environmentMap.sample(ray.direction));
		}
		
	}
}//End render row	

glm::vec3 App::refractedColor(Model& model, Ray ray) {

	Ray finalRay = ray;

	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;

	bool inModel = false;

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