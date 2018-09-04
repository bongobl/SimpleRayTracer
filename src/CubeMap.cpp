#include "../include/CubeMap.h"

CubeMap::CubeMap() {

}
CubeMap::CubeMap(Image right_face, Image left_face, Image top_face, Image bottom_face, Image front_face, Image back_face){
	
	rightFace = right_face;
	leftFace = left_face;
	topFace = top_face;
	bottomFace = bottom_face;
	frontFace = front_face;
	backFace = back_face;
}

glm::vec3 CubeMap::sample(glm::vec3 param) const{

	int toSampleID;
	float uParam,vParam;

	//if X component is largest 
	if(abs(param.x) >= abs(param.y) && abs(param.x) >= abs(param.z)){
		param.y /= abs(param.x);
		param.z /= abs(param.x);
		param.x /= abs(param.x);
	
		if(param.x > 0){
			//Sample Right Face
			toSampleID = RIGHT;
			uParam = paramaterized(param.z);
			vParam = paramaterized(param.y);
		}else{
			//Sample Left Face
			toSampleID = LEFT;
			uParam = 1 - paramaterized(param.z);
			vParam = paramaterized(param.y);
		}

	//if Y component is largest
	}else if(abs(param.y) > abs(param.x) && abs(param.y) >= abs(param.z)){
		param.x /= abs(param.y);	
		param.z /= abs(param.y);
		param.y /= abs(param.y);

		if(param.y > 0){
			//Sample Top Face
			toSampleID = TOP;
			uParam = paramaterized(param.x);
			vParam = paramaterized(param.z);
		}else{
			//Sample Bottom Face
			toSampleID = BOTTOM;
			uParam = paramaterized(param.x);
			vParam = 1 - paramaterized(param.z);
		}

	//if Z component is largest
	}else if(abs(param.z) > abs(param.x) && abs(param.z) > abs(param.y)){
		param.x /= abs(param.z);
		param.y /= abs(param.z);
		param.z /= abs(param.z);

		if(param.z > 0){
			//Sample Front Face
			toSampleID = FRONT;
			uParam = 1 - paramaterized(param.x);
			vParam = paramaterized(param.y);
		}else{
			//Sample Back Face
			toSampleID = BACK;
			uParam = paramaterized(param.x);
			vParam = paramaterized(param.y);
		}
	}

	glm::vec2 uvSampler = glm::vec2(uParam, vParam);

	glm::vec3 finalColorValue;

	//perform 2D sample on proper face
	if(toSampleID == RIGHT){
		finalColorValue = rightFace.sample(uvSampler);
	}else if(toSampleID == LEFT){
		finalColorValue = leftFace.sample(uvSampler);
	}else if(toSampleID == TOP){
		finalColorValue = topFace.sample(uvSampler);
	}else if(toSampleID == BOTTOM){
		finalColorValue = bottomFace.sample(uvSampler);
	}else if(toSampleID == FRONT){
		finalColorValue = frontFace.sample(uvSampler);
	}else if(toSampleID == BACK){
		finalColorValue = backFace.sample(uvSampler);
	}
	return finalColorValue;
}

//private helpers

float CubeMap::paramaterized(float raw) const{
	return raw / 2 + 0.5f;
}