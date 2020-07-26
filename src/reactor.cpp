#include "reactor.h"
#include "model/scene.h"
#include "model/terrain.h"

namespace reactor {

struct Visitor {
	entt::registry* registry;
	model::Scene* scene;
	model::Terrain* terrain;

	void visit(SelectEntity cmd) const {
		std::puts("Select!");

		registry->clear<Selected>();
		registry->emplace<Selected>(cmd.entity);
	}

	void visit(ToggleAffectsTerrain cmd) const {
		std::puts("Toggle affects terrain!");

		if (registry->has<AffectsTerrain>(cmd.entity)) {
			registry->remove<AffectsTerrain>(cmd.entity);
		} else {
			registry->emplace<AffectsTerrain>(cmd.entity);
		}
	}

	void visit(MoveEntity cmd) const {
		std::puts("Move!");
		registry->replace<Position>(cmd.entity, cmd.new_position);

		if (registry->has<AffectsTerrain>(cmd.entity)) {
			const auto size = registry->get<Size>(cmd.entity);
			terrain->mark_dirty(cmd.old_position, size);
			terrain->mark_dirty(cmd.new_position, size);
		}
	}
};


void Reactor::queue(Command cmd) {
	this->command_queue.push_back(cmd);
}


void Reactor::react(model::Scene& scene, model::Terrain& terrain) {
	swap(this->command_queue, this->back_command_queue);

	Visitor visitor {&scene.registry, &scene, &terrain};

	for (auto&& command : this->back_command_queue) {
		std::visit(
			[visitor] (auto&& cmd) { visitor.visit(std::move(cmd)); },
			std::move(command)
		);
	}

	this->back_command_queue.clear();
}


} // namespace reactor
