#include "view/entity_list.h"
#include "model/scene.h"
#include "reactor.h"

#include <SDL2/SDL.h>

namespace view::entity_list {

const int ENTRY_SIZE = 50;
const int ENTRY_COLS = 4;
const int PANEL_WIDTH = ENTRY_SIZE * ENTRY_COLS;
const int SELECTION_WIDTH = 5;


bool EntityList::handle_mouse_down(int sx, int sy, const model::Scene& scene, reactor::Reactor& reactor) {
	auto view = scene.registry.view<const Color>();

	if (sy > int(view.size()+ENTRY_COLS-1)/ENTRY_COLS * ENTRY_SIZE) { return false; }
	if (sx < this->render_w - PANEL_WIDTH) { return false; }

	// HACK: all of this should be made persistent
	const auto index = sy/ENTRY_SIZE*ENTRY_COLS + (PANEL_WIDTH - (this->render_w - sx))/ENTRY_SIZE;
	if (index >= int(view.size())) {
		return false;
	}

	const auto ent = view.begin() + index;
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
			this->render_w - PANEL_WIDTH + (index%ENTRY_COLS)*ENTRY_SIZE, index/ENTRY_COLS * ENTRY_SIZE,
			ENTRY_SIZE, ENTRY_SIZE,
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
				this->render_w - PANEL_WIDTH + (index%ENTRY_COLS)*ENTRY_SIZE, index/ENTRY_COLS * ENTRY_SIZE,
				SELECTION_WIDTH, ENTRY_SIZE,
			};

			SDL_SetRenderDrawColor(renderer, 230, 30, 30, 255);
			SDL_RenderFillRect(renderer, &draw_rect);
		}

		index++;
	}
}



}