#include "view/entity_list.h"
#include "model/scene.h"
#include "reactor.h"

#include <SDL2/SDL.h>

namespace view::entity_list {

const int ENTRY_WIDTH = 100;
const int ENTRY_HEIGHT = 20;
const int SELECTION_WIDTH = 5;


bool EntityList::handle_mouse_down(int sx, int sy, const model::Scene& scene, reactor::Reactor& reactor) {
	auto view = scene.registry.view<const Color>();

	if (sy > view.size() * ENTRY_HEIGHT) { return false; }
	if (sx < this->render_w - ENTRY_WIDTH) { return false; }

	// HACK: all of this should be made persistent anyway
	const auto ent = view.begin() + sy / ENTRY_HEIGHT;
	reactor.queue(reactor::SelectEntity {*ent});
	return true;
}


void EntityList::render(SDL_Renderer* renderer, const model::Scene& scene) {
	SDL_GetRendererOutputSize(renderer, &this->render_w, nullptr);

	int index = 0;

	auto view = scene.registry.view<const Color>();
	for (auto entity : view) {
		const auto color = view.get<const Color>(entity);
		const bool selected = scene.registry.has<Selected>(entity);

		const SDL_Rect draw_rect {
			this->render_w - ENTRY_WIDTH, index * ENTRY_HEIGHT,
			ENTRY_WIDTH, ENTRY_HEIGHT,
		};

		SDL_SetRenderDrawColor(
			renderer,
			int(color.r*255.0f),
			int(color.g*255.0f),
			int(color.b*255.0f),
			255
		);

		SDL_RenderFillRect(renderer, &draw_rect);

		if (selected) {
			const SDL_Rect draw_rect {
				this->render_w - ENTRY_WIDTH, index * ENTRY_HEIGHT,
				SELECTION_WIDTH, ENTRY_HEIGHT,
			};

			SDL_SetRenderDrawColor(renderer, 230, 30, 30, 255);
			SDL_RenderFillRect(renderer, &draw_rect);
		}

		index++;
	}
}



}