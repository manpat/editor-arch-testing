#pragma once

#include <vector>
#include <variant>
#include <entt/entity/entity.hpp>

#include "components.h"

namespace model {
	struct Scene;
	struct Terrain;
}

namespace reactor {
	struct SelectEntity { entt::entity entity; };
	struct ToggleAffectsTerrain { entt::entity entity; };

	struct MoveEntity {
		entt::entity entity;
		Position old_position;
		Position new_position;
	};


	using Command = std::variant<
		SelectEntity,
		ToggleAffectsTerrain,
		MoveEntity
	>;

	struct Reactor {
		void queue(Command);
		void react(model::Scene&, model::Terrain&);

	private:
		std::vector<Command> command_queue;
		std::vector<Command> back_command_queue;
	};
}
