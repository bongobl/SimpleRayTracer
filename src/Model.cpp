#include <cstdlib>
#include "../include/Model.h"


Model::Model(string model_filename){

	//Transform (not used yet)
	position = glm::vec3(0,0,0);
	orientation = glm::mat4(1.0f);
	scale = glm::vec3(1,1,1);
	toWorld = glm::mat4(1.0f);
	toWorld3x3 = glm::mat3(1.0f);

	//Load Mesh
	FILE* fp;
	char currLine[BUFSIZ];
	float x, y, z;
	unsigned int v1, v2, v3, n1, n2, n3, t1, t2, t3;

	fp = fopen(model_filename.c_str(), "rb");
	if (fp == NULL) {
		std::cerr << "***Error loading file " << model_filename << "***" << std::endl;
		system("pause");
		exit(-1);
	}

	std::cout << "Parsing " << model_filename << "..." << std::endl;
	

	while (fgets(currLine, BUFSIZ, fp) != NULL) {

		//Process Vertex Attributes
		if (currLine[0] == 'v') {

			//read vertex normal (normals are read first, so only push back vector here)
			if (currLine[1] == 'n' && currLine[2] == ' ') {
				sscanf(currLine, "vn %f %f %f", &x, &y, &z);
				vertices.push_back( new Vertex(glm::vec3(0,0,0), glm::vec2(0,0), glm::vec3(x,y,z)) );
			}

			//read in texCoords
			else if (currLine[1] == 't' && currLine[2] == ' ') {
				sscanf(currLine, "vt %f %f", &x, &y);
				vertices.back()->texCoord = glm::vec2(x, y);

			}
			//read vertex position
			else if (currLine[1] == ' ') {
				sscanf(currLine, "v %f %f %f", &x, &y, &z);
				vertices.back()->position = glm::vec3(x,y,z);
			}	
		}

		//Process Triangles
		else if (currLine[0] == 'f' && currLine[1] == ' ') {
			sscanf(currLine, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3);
			faces.push_back( Triangle(vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1]) );
		}

	}//END WHILE
	

	//report model stats
	cout << "Loaded Model " << model_filename << " with " << vertices.size() << " vertices and " << faces.size() << " faces" << endl;

	//Deep copy vertices to rawVertices
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		rawVertices.push_back(new Vertex(*vertices.at(i)));
	}

	//close file after reading
	fclose(fp);

	
} 

Model::~Model() {

	//clean up vertices
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		if (vertices.at(i) != NULL) {
			delete vertices.at(i);
			vertices.at(i) = NULL;
		}
	}

	//clean up raw vertices
	for (unsigned int i = 0; i < rawVertices.size(); ++i) {
		if (rawVertices.at(i) != NULL) {
			delete rawVertices.at(i);
			rawVertices.at(i) = NULL;
		}
	}
}
//position
void Model::setPosition(const glm::vec3 model_position) {
	position = model_position;
	updateToWorldAndMesh();
}
glm::vec3 Model::getPosition() const {
	return position;
}

//orientation
void Model::setOrientation(const glm::mat4 model_orientation) {
	orientation = model_orientation;
	updateToWorldAndMesh();
}
glm::mat4 Model::getOrientation() const {
	return orientation;
}

//scale
void Model::setScale(const glm::vec3 model_scale) {
	scale = model_scale;
	updateToWorldAndMesh();
}
glm::vec3 Model::getScale() const {
	return scale;
}

//toWorld
glm::mat4 Model::getToWorld() const{
	return toWorld;
}
glm::mat3 Model::getToWorld3x3() const{
	return toWorld3x3;
}

//Private 
void Model::updateToWorldAndMesh() {

	//update toWorld and toWorld3x3 matrix
	toWorld = glm::translate(glm::mat4(1.0f), position) * orientation * glm::scale(glm::mat4(1.0f), scale);
	toWorld3x3 = glm::transpose(glm::inverse(toWorld));

	//loop through mesh and update vertices to reflect toWorld transformation
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		if(vertices.at(i) != NULL){
			vertices.at(i)->position = toWorld * glm::vec4(rawVertices.at(i)->position, 1);
			vertices.at(i)->setNormal(toWorld3x3 * rawVertices.at(i)->getNormal());
		}
		else{
			cerr << "Model: trying to access vertex from vertices array, but pointer is NULL" << endl;
		}
	}
}