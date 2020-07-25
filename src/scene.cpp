#include "scene.h"
#include "components.h"

void Scene::update() {
	// Wrap around scene
	registry.view<Position, const Size>().each(
		[this] (auto& pos, const auto& size) {
			if (pos.x < 0.0f) {
				pos.x = world_w - size.w;
			} else if (pos.x + size.w > world_w) {
				pos.x = 0.0f;
			} 

			if (pos.y < 0.0f) {
				pos.y = world_h - size.h;
			} else if (pos.y + size.h > world_h) {
				pos.y = 0.0f;
			}
		}
	);
}