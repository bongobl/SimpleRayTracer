#include "../include/MeshPool.h"
#include "../include/Model.h"
void MeshPool::addModel(Model* model) {
	//add model to model list
	allModels.push_back(model);
	
	//update all triangles to include new model
	for (unsigned int i = 0; i < model->faces.size(); ++i) {
		allFaces.push_back(model->faces[i]);
	}
}