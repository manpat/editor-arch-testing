#pragma once

struct SDL_Renderer;

namespace model {
	struct Scene;
}
namespace reactor {
	struct Reactor;
}


namespace view::entity_list {
	struct EntityList {
		bool handle_mouse_down(int sx, int sy, const model::Scene&, reactor::Reactor&);
		void render(SDL_Renderer*, const model::Scene&);
	private:
		int render_w;
	};
}