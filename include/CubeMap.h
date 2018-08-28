#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Image.h"

class CubeMap{
	
	Image rightFace;
	Image leftFace;
	Image topFace;
	Image bottomFace;
	Image frontFace;
	Image backFace;
	

public:

	CubeMap(Image right_face, Image left_face, Image top_face, Image bottom_face, Image front_face, Image back_face);

	enum FaceIDs{RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK};
	glm::vec3 sample(glm::vec3 param);

private:
	float paramaterized(float raw);
};