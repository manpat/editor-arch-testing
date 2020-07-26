#pragma once

#include <entt/entt.hpp>

namespace model {
	struct Scene {
		entt::registry registry {};
		float world_size {20.0f};

		void update();
	};
}

