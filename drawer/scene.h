#pragma once
#include "primitive.h"
#include "light.h"
#include <vector>

class Scene {
public:
	Scene() {};
	~Scene();
	
	std::vector<Primitive*> primitives;
	Light* light;
	
};