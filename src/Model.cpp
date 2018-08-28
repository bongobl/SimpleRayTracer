#include <cstdlib>
#include "../include/Model.h"


Model::Model(string model_filename){
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
	
	//to store all vertices from which triangles are later defined
	vector<Vertex> vertices;

	while (fgets(currLine, BUFSIZ, fp) != NULL) {

		//Process Vertex Attributes
		if (currLine[0] == 'v') {

			//read vertex normal (normals are read first, so only push back vector here)
			if (currLine[1] == 'n' && currLine[2] == ' ') {
				sscanf(currLine, "vn %f %f %f", &x, &y, &z);
				vertices.push_back( Vertex(glm::vec3(0,0,0), glm::vec2(0,0), glm::vec3(x,y,z)) );
			}

			//read in texCoords
			else if (currLine[1] == 't' && currLine[2] == ' ') {
				sscanf(currLine, "vt %f %f", &x, &y);
				vertices.back().texCoord = glm::vec2(x, y);
				//cout << vertices.back().texCoord.x << "    " << vertices.back().texCoord.y << endl;
			}
			//read vertex position
			else if (currLine[1] == ' ') {
				sscanf(currLine, "v %f %f %f", &x, &y, &z);
				vertices.back().position = glm::vec3(x,y,z);
			}	
		}


		//Process Triangles
		else if (currLine[0] == 'f' && currLine[1] == ' ') {
			sscanf(currLine, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3);
			faces.push_back( Triangle(vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1]) );
		}

	}//END WHILE
	

	//debug
	cout << "Loaded Model " << model_filename << " with " << vertices.size() << " vertices and " << faces.size() << " faces" << endl;

	//don't need vertices list anymore
	vertices.clear();

	//close file after reading
	fclose(fp);

	
} 