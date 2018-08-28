#include "../include/Material.h"

Material::Material(){
	diffuse = glm::vec3(1,1,1);
	specular = glm::vec3(1,1,1);
	ambient = glm::vec3(0.05f,0.05f,0.05f);
}
Material::Material(Image surface_texture) : surfaceTexture(surface_texture){
	diffuse = glm::vec3(1,1,1);
	specular = glm::vec3(1,1,1);
	ambient = glm::vec3(0.01f,0.01f,0.01f);
}
Material::Material(Image surface_texture, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _ambient){
	surfaceTexture = surface_texture;
	diffuse = _diffuse;
	specular = _specular;
	ambient = _ambient;

}