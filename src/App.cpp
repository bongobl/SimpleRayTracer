#include "../include/App.h"

App::App() :	sphere("resources/ObjModels/sphere.obj"),
				ico("resources/ObjModels/ico.obj")
{
	//create output image texture
	outputImage.setAsWrite(OUTPUT_WIDTH, OUTPUT_HEIGHT);

}

void App::init() {


	//create environment map
	environmentMap.rightFace.setAsRead("resources/textures/CubeMap/right.jpg");
	environmentMap.leftFace.setAsRead("resources/textures/CubeMap/left.jpg");
	environmentMap.topFace.setAsRead("resources/textures/CubeMap/top.jpg");
	environmentMap.bottomFace.setAsRead("resources/textures/CubeMap/bottom.jpg");
	environmentMap.frontFace.setAsRead("resources/textures/CubeMap/front.jpg");
	environmentMap.backFace.setAsRead("resources/textures/CubeMap/back.jpg");


	//create material
	Material basicMaterial;
	basicMaterial.color = glm::vec3(1,1,1);
	basicMaterial.reflectNewRay = true;
	basicMaterial.refractNewRay = true;
	basicMaterial.refractiveIndex = 1.03f;
	basicMaterial.reflectToRefractParam = 0.7f;

	//assign material to sphere
	sphere.material = basicMaterial;

	//define ico material
	ico.material.color = glm::vec3(0,1,1);

	//enter all models
	allModels.push_back(&sphere);
	allModels.push_back(&ico);

	//transform models
	sphere.setScale(glm::vec3(1.5f,1.5f,1.5f));
	sphere.setPosition(glm::vec3(-50,0,0));
	ico.setScale(glm::vec3(1.2f,1.2f,1.2f));
	ico.setPosition(glm::vec3(6,100,-500));

	//define light
	directionalLight.setDirection(glm::vec3(0.5f,-1,-1));

}

void App::dispose() {

	//clean up CubeMap
	environmentMap.rightFace.dispose();
	environmentMap.leftFace.dispose();
	environmentMap.topFace.dispose();
	environmentMap.bottomFace.dispose();
	environmentMap.frontFace.dispose();
	environmentMap.backFace.dispose();


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


	for (int x = 0; x < OUTPUT_WIDTH; ++x) {

		//get ray from camera based on current pixel
		Ray rayFromCamera = Camera::getRay(x, yVal);

		//fire ray into scene and get final pixel color
		glm::vec3 pixelColor = getColorFromScene(rayFromCamera, 0);

		//set pixel in output image to that color
		outputImage.setPixel(x, yVal, pixelColor);

	}//For Each Pixel


}//End render row	

glm::vec3 App::getColorFromScene(Ray ray, int numBounces) {

	//base case, max num bounces, just sample environment map
	if(numBounces >= MAX_NUM_RAY_BOUNCES){
		return environmentMap.sample(ray.direction);
	}

	//interpolated vertex attributes
	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;
	Model* currModel = NULL;

	//fire ray into scene to find color for pixel
	if (ray.intersectMesh(allModels, fragPosition, fragTexCoord, fragNormal, currModel)) {


		//get current model material
		Material currMaterial = currModel->material;

		//sample surface texture
		glm::vec3 surfaceTextureColor = glm::vec3(1, 1, 1) - (currMaterial.surfaceTextureStrength *
			(glm::vec3(1, 1, 1) - currMaterial.surfaceTexture.sample(fragTexCoord)));


		//get color from reflected ray
		glm::vec3 colorFromReflectedRay =  glm::vec3(1,1,1);
		if(currMaterial.reflectNewRay){

			Ray reflectedRay;
			reflectedRay.origin = fragPosition;
			reflectedRay.direction = glm::reflect(ray.direction, fragNormal);
			colorFromReflectedRay = getColorFromScene(reflectedRay, numBounces + 1);
			
		}

		//get color from reflected ray
		//NOTE: Hard-coded for 2 bounces. Only works when models are not intersecting
		glm::vec3 refractionColor;
		if(currMaterial.refractNewRay){
						
			glm::vec3 oppFragPosition;
			glm::vec2 oppFragTexCoord;
			glm::vec3 oppFragNormal;
			Model* oppModel = NULL;

			Ray refractedRay;
			refractedRay.origin = fragPosition;
			refractedRay.direction = RenderUtils::refract(ray.direction, fragNormal, 1, currMaterial.refractiveIndex);

			//go out model (theoretically, if statement should always be true)
			if(refractedRay.intersectMesh(allModels, oppFragPosition, oppFragTexCoord, oppFragNormal, oppModel)){
				refractedRay.origin = oppFragPosition;
				refractedRay.direction = RenderUtils::refract(refractedRay.direction, oppFragNormal, 1, currMaterial.refractiveIndex);
			}
			refractionColor = getColorFromScene(refractedRay, numBounces + 1);
			
		}
		

		//calculate diffuse
		glm::vec3 diffuseComponent = max(glm::dot(fragNormal, -1.0f * directionalLight.getDirection()), 0.0f) * currMaterial.diffuse;

		//calculate specular
		glm::vec3 reflection = glm::reflect(directionalLight.getDirection(), fragNormal);
		glm::vec3 toEye = glm::normalize(ray.origin - fragPosition);
		glm::vec3 specularComponent = (float)pow(max(glm::dot(reflection, toEye), 0.0f), 20) * currMaterial.specular;

		//calculate ambient
		glm::vec3 ambientComponent = currMaterial.ambient;

		//combine reflection components			
		glm::vec3 totalReflectionColor = colorFromReflectedRay * surfaceTextureColor * (diffuseComponent + specularComponent + ambientComponent);


		//define refLeft based on param and normal's relation to camera
		float refLerp = 0;	//pure reflective (no refraction unless material says to refract ray)

		//if material refracts light, we need to calculate reflect <-> refract lerp
		if(currMaterial.refractNewRay){
			float normalDotFragToCam = glm::pow(glm::max(glm::dot(glm::normalize(ray.origin - fragPosition), fragNormal), 0.0f), 0.7f);

			if (currMaterial.reflectToRefractParam <= 0.5f) {
				refLerp = 2 * currMaterial.reflectToRefractParam * normalDotFragToCam;
			}
			else {
				refLerp = 1 - 2 * (1 - currMaterial.reflectToRefractParam) * (1 - normalDotFragToCam);
			}
		}

		//lerp between reflectivness and refractivness
		glm::vec3 finalColor = currMaterial.color * ((1 - refLerp) * totalReflectionColor + (refLerp)* refractionColor);

		//set image pixel value
		
		return finalColor;

	}
	else {

		return environmentMap.sample(ray.direction);
	}
}