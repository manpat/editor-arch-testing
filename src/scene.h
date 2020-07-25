#pragma once

#include <entt/entt.hpp>


struct Scene {
	entt::registry registry {};
	float world_w {20.0f}, world_h {20.0f};

	void update();
};
