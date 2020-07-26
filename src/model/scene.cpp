#include "model/scene.h"
#include "components.h"

namespace model {

void Scene::update() {
	// Wrap around scene
	registry.view<Position, const Size>().each(
		[this] (auto& pos, const auto& size) {
			if (pos.x < 0.0f) {
				pos.x = world_size - size.w;
			} else if (pos.x + size.w > world_size) {
				pos.x = 0.0f;
			} 

			if (pos.y < 0.0f) {
				pos.y = world_size - size.h;
			} else if (pos.y + size.h > world_size) {
				pos.y = 0.0f;
			}
		}
	);
}

} // namespace model
