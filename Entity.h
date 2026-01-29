#pragma once
/*
* Entities are items rendered on screen
* Each item has its own geometry (Mesh) and material (shaders and input layout -> including texture and sampler states to be created + binded)
* Shaders need to be created at start up, and set before the item is drawn to the screen
*/ 
#include "Mesh.h"
#include "Material.h"
#include "Graphics.h"

class Entity 
{

public:
	Entity(std::shared_ptr<Mesh> inMesh, std::shared_ptr<Material> inMaterial);
	
	void Draw();

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

};
