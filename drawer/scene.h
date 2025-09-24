#pragma once
#include "primitive.h"
#include "light.h"
#include "geometry/model.h"
#include <vector>

class Scene {
public:
	Scene() {};
	~Scene() {};
	
	std::vector<Primitive*> primitives;
	std::vector<Model*> models;
	Light* light;
	
};