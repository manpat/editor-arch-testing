#pragma once

#include <entt/entt.hpp>

struct Scene;

struct Terrain {
	entt::observer affects_terrain_observer;
	entt::observer transform_change_observer;

	Terrain(Scene&);
	void update();
};