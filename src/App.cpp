#include "../include/App.h"

App::App() :	cube("resources/ObjModels/FilletCube.obj"),
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
	basicMaterial.refractiveIndex = 1.08f;
	basicMaterial.reflectToRefractParam = 0.5f;
	basicMaterial.internalColor = glm::vec3(0,1,0);

	//assign material to sphere
	ico.material = basicMaterial;

	//define ico material
	cube.material.color = glm::vec3(0.7f,0,0.7f);

	//enter all models
	allModels.push_back(&cube);
	allModels.push_back(&ico);

	//transform models
	cube.setScale(glm::vec3(0.56f,0.56f,0.56f));
	ico.setScale(glm::vec3(1.8f,1.8f,1.8f));

	//define light
	directionalLight.setDirection(glm::vec3(-0.5f,-1,-1));

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

	//iterate through entire row
	for (int x = 0; x < OUTPUT_WIDTH; ++x) {

		//get ray from camera based on current pixel
		Ray rayFromCamera = Camera::getRay(x, yVal);

		//store distance from ray to mesh
		float distanceToMesh;

		//fire ray into scene and get final pixel color
		glm::vec3 pixelColor = getColorFromScene(rayFromCamera, distanceToMesh);

		//set pixel in output image to that color
		outputImage.setPixel(x, yVal, pixelColor);


	}//For Each Pixel


}//End render row	

glm::vec3 App::getColorFromScene(const Ray ray, float &distToMesh) {

	//base case, max num bounces, just sample environment map
	if(ray.timesBounced >= MAX_NUM_RAY_BOUNCES){
		return environmentMap.sample(ray.direction);
	}

	//interpolated vertex attributes
	glm::vec3 fragPosition;
	glm::vec2 fragTexCoord;
	glm::vec3 fragNormal;
	Model* currModel = NULL;

	//fire ray into scene to find color for pixel
	if (ray.intersectMesh(allModels, fragPosition, fragTexCoord, fragNormal, currModel)) {

		//calc distance from ray origin to mesh
		distToMesh = glm::length(ray.origin - fragPosition);

		//for recursive rays
		float distFromMesh;

		//get current model material
		Material currMaterial = currModel->material;

		
		//get color from reflected ray
		glm::vec3 refractionColor;
		if(currMaterial.refractNewRay){
				
			Ray refractedRay;
			refractedRay.origin = fragPosition;
			refractedRay.direction = RenderUtils::refract(ray.direction, fragNormal, 1, currMaterial.refractiveIndex);
			refractedRay.timesBounced = ray.timesBounced + 1;
			refractionColor = getColorFromScene(refractedRay, distFromMesh);


			//TOGGLE AT WILL: Longer internal rays will lerp more towards material internal color
			if(glm::dot(fragNormal,ray.direction) < 0){
				float param = min(1.0f,distFromMesh / currMaterial.totalOpaqueDistance);
				refractionColor = (1 - param) * refractionColor + param * currMaterial.internalColor;
			} 
			
		}
		
		//if ray intersects from back face of triangle, return pure refractive color
		if(glm::dot(fragNormal,ray.direction) > 0){
			return refractionColor;
		}

		//get color from reflected ray
		glm::vec3 colorFromReflectedRay =  glm::vec3(1,1,1);
		if(currMaterial.reflectNewRay){

			Ray reflectedRay;
			reflectedRay.origin = fragPosition;
			reflectedRay.direction = glm::reflect(ray.direction, fragNormal);
			reflectedRay.timesBounced = ray.timesBounced + 1;
			colorFromReflectedRay = getColorFromScene(reflectedRay, distFromMesh);
			
		}

		//sample surface texture
		glm::vec3 surfaceTextureColor = glm::vec3(1, 1, 1) - (currMaterial.surfaceTextureStrength *
			(glm::vec3(1, 1, 1) - currMaterial.surfaceTexture.sample(fragTexCoord)));


		
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

		//return color for this ray
		return finalColor;

	}//End Ray Intersect Mesh
	
	distToMesh = -1;
	//Ray Doesn't hit any mesh, sample environment map
	return environmentMap.sample(ray.direction);
}