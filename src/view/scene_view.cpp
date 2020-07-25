#include "view/scene_view.h"
#include "components.h"
#include "scene.h"

#include <SDL2/SDL.h>

std::pair<int, int> SceneView::to_screen(Position pos) const {
	return {
		int((pos.x - camera_x)*camera_scale + render_w/2.0f),
		int((pos.y - camera_y)*camera_scale + render_h/2.0f)
	};
}

std::pair<int, int> SceneView::to_screen(Size size) const {
	return {
		int(size.w*camera_scale),
		int(size.h*camera_scale)
	};
}

void SceneView::render(SDL_Renderer* renderer, const Scene& scene) {
	SDL_GetRendererOutputSize(renderer, &render_w, &render_h);

	// Draw scene background
	{
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

		auto [screen_x, screen_y] = to_screen(Position {0.0f, 0.0f});
		auto [screen_w, screen_h] = to_screen(Size {scene.world_w, scene.world_h});

		const SDL_Rect draw_rect {
			screen_x, screen_y,
			screen_w, screen_h,
		};

		SDL_RenderFillRect(renderer, &draw_rect);
	}

	// Draw entities
	auto view = scene.registry.view<const Position, const Size, const Color>();
	for (auto entity : view) {
		auto [position, size, color] = view.get<const Position, const Size, const Color>(entity);

		auto [screen_x, screen_y] = to_screen(position);
		auto [screen_w, screen_h] = to_screen(size);

		const SDL_Rect draw_rect {
			screen_x, screen_y,
			screen_w, screen_h,
		};


		SDL_SetRenderDrawColor(
			renderer,
			int(color.r*255.0f),
			int(color.g*255.0f),
			int(color.b*255.0f),
			255
		);

		SDL_RenderFillRect(renderer, &draw_rect);
	}
}