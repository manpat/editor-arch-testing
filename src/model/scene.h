#pragma once

#include <entt/entt.hpp>

namespace model {
	struct Scene {
		Scene(float world_size);

		entt::registry registry {};
		float world_size {};

		void update();
	};
}

