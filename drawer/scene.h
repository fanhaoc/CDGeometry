#pragma once
#include "primitives/superPrimitive.h"
#include "light.h"
#include "geometry/model.h"
#include <vector>

class Scene {
public:
	Scene() {};
	~Scene() {};
	
	std::vector<SuperPrimitive*> primitives;
	std::vector<Model*> models;
	Light* light;
	
};