#include "model/terrain.h"
#include "model/scene.h"
#include "components.h"

#include <algorithm>
#include <cmath>

namespace model {

Terrain::Terrain(Scene& scene, int resolution)
	: data{std::make_unique<float[]>(resolution*resolution)}
	, resolution{resolution}
	, scene_to_data{float(resolution)/scene.world_size}
{
	std::fill(this->data.get(), this->data.get() + this->resolution*this->resolution, 0.0f);
}


void Terrain::mark_dirty(Position, Size) {
	// this is mostly for demonstrative purposes
	this->dirty = true;
}


void Terrain::update(const Scene& scene) {
	if (!this->dirty) {
		return;
	}

	this->dirty = false;

	std::fill(this->data.get(), this->data.get() + this->resolution*this->resolution, 0.0f);

	auto view = scene.registry.view<const Position, const Size, const AffectsTerrain>();
	for (auto entity : view) {
		const auto& [pos, size] = view.get<const Position, const Size>(entity);

		const auto [cx, cy] = pos;
		const auto [w, h] = size;

		const auto left = int(std::floor(cx*this->scene_to_data));
		const auto right = int(std::ceil((cx+w)*this->scene_to_data));
		const auto top = int(std::floor(cy*this->scene_to_data));
		const auto bottom = int(std::ceil((cy+h)*this->scene_to_data));

		for (int y = std::max(0, top); y < std::min(this->resolution, bottom); y++) {
			for (int x = std::max(0, left); x < std::min(this->resolution, right); x++) {
				this->data[x + y * this->resolution] = 1.0f;
			}
		}
	}
}

float Terrain::get(int x, int y) const {
	if (x < 0 || x >= this->resolution) { return 0.0f; }
	if (y < 0 || y >= this->resolution) { return 0.0f; }
	return this->data[x + y * this->resolution];
}


} // namespace model