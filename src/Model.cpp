#include <cstdlib>
#include <map>
#include "../include/Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Model::Model(string model_filename){

	//Transform (not used yet)
	position = glm::vec3(0,0,0);
	orientation = glm::mat4(1.0f);
	scale = glm::vec3(1,1,1);
	toWorld = glm::mat4(1.0f);
	toWorld3x3 = glm::mat3(1.0f);
	
	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_filename.c_str())) {
        throw std::runtime_error("Failed to load model " + model_filename);
    }

    
    std::map<Vertex,unsigned int> uniqueVertices;
    std::vector<unsigned int> indexBuffer;


    for(const auto& shape : shapes){
    	for(const auto& index : shape.mesh.indices){
    		Vertex tentative;

    		//read in position
    		tentative.position = glm::vec3(
    			attrib.vertices[3 * index.vertex_index + 0],
    			attrib.vertices[3 * index.vertex_index + 1],
    			attrib.vertices[3 * index.vertex_index + 2]
    		);

    		//read in normal
    		tentative.setNormal(glm::vec3(
    			attrib.normals[3 * index.normal_index + 0],
    			attrib.normals[3 * index.normal_index + 1],
    			attrib.normals[3 * index.normal_index + 2]
    		));

    		//red in UV texCoords
    		tentative.texCoord = glm::vec2(
    			attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
    		);

    		//if vertex is new, add to map with index being current size of vertices array
    		if(uniqueVertices.count(tentative) == 0){
    			uniqueVertices[tentative] = vertices.size();
    			indexBuffer.push_back(vertices.size());
    			vertices.push_back(new Vertex(tentative));

    		}else{
    			indexBuffer.push_back(uniqueVertices[tentative]);
    		}
		

    	}
    }

    //use index buffer to construct all triangles
    for(unsigned int i = 0; i < indexBuffer.size(); i += 3){
    	Triangle currTriangle(vertices[indexBuffer[i]], vertices[indexBuffer[i + 1]], vertices[indexBuffer[i + 2]]);
    	faces.push_back(currTriangle);
    }

    //store model space vertices for calculating toWorld of vertices during transformation
    for (unsigned int i = 0; i < vertices.size(); ++i) {
		rawVertices.push_back(new Vertex(*vertices.at(i)));
	}
	cout << "Loaded model " << model_filename << endl;
	
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