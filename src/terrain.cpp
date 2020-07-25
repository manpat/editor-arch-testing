#include "terrain.h"
#include "scene.h"
#include "components.h"

Terrain::Terrain(Scene& scene)
	: affects_terrain_observer {
		scene.registry,
		entt::collector.group<AffectsTerrain>()
	}
	, transform_change_observer {
		scene.registry,
		entt::collector.update<Position>().where<AffectsTerrain>()
			.update<Size>().where<AffectsTerrain>()
	}
{
}


void Terrain::update() {
	for (auto entity : this->transform_change_observer) {
		std::puts("[terrain] transform change!");
	}

	for (auto entity : this->affects_terrain_observer) {
		std::puts("[terrain] affects terrain change!");
	}

	this->transform_change_observer.clear();
	this->affects_terrain_observer.clear();
}