#include "view/scene_view.h"
#include "components.h"
#include "reactor.h"
#include "model/scene.h"

#include <SDL2/SDL.h>
#include <algorithm>



namespace view::scene {


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


Position SceneView::to_position(int sx, int sy) const {
	return {
		(float(sx) - render_w/2.0f)/camera_scale + camera_x,
		(float(sy) - render_h/2.0f)/camera_scale + camera_y
	};
}


Size SceneView::to_size(int sx, int sy) const {
	return {
		float(sx)/camera_scale,
		float(sy)/camera_scale,
	};
}


bool SceneView::handle_mouse_down(int sx, int sy, const model::Scene& scene, reactor::Reactor&) {
	const auto mpos = to_position(sx, sy);

	auto view = scene.registry.view<const Position, const Size>();
	auto entity_under_mouse = std::find_if(view.begin(), view.end(), [&view, mpos] (auto entity) {
		auto [pos, size] = view.get<const Position, const Size>(entity);

		const auto extent_x = size.w/2.0f;
		const auto extent_y = size.h/2.0f;

		const auto dx = pos.x + extent_x - mpos.x;
		const auto dy = pos.y + extent_y - mpos.y;

		return std::abs(dx) <= extent_x && std::abs(dy) <= extent_y;
	});

	if (entity_under_mouse != view.end()) {
		this->in_progress_mouse_data = InProgressMouseData {
			sx, sy, 0, 0,
			DragState::Stationary,
			*entity_under_mouse
		};

		return true;
	}

	return false;
}


bool SceneView::handle_mouse_move(int sx, int sy, const model::Scene& scene, reactor::Reactor& reactor) {
	if (!this->in_progress_mouse_data) {
		return false;
	}

	auto& [start_x, start_y, delta_x, delta_y, drag_state, entity_maybe] = *this->in_progress_mouse_data;

	delta_x = (sx - start_x);
	delta_y = (sy - start_y);

	const auto travel_distance = std::abs(delta_x) + std::abs(delta_y);

	if (travel_distance > 5) {
		drag_state = DragState::Dragging;

		if (entity_maybe && !scene.registry.has<Selected>(*entity_maybe)) {
			reactor.queue(reactor::SelectEntity {*entity_maybe});
		}
	} 

	return true;
}


bool SceneView::handle_mouse_up(int sx, int sy, const model::Scene& scene, reactor::Reactor& reactor) {
	if (!this->in_progress_mouse_data) {
		return false;
	}

	const auto& [start_x, start_y, delta_x, delta_y, drag_state, entity_maybe] = *this->in_progress_mouse_data;
	if (!entity_maybe) {
		this->in_progress_mouse_data = std::nullopt;
		return true;
	}

	const auto entity = *entity_maybe;

	if (drag_state == DragState::Stationary) {
		reactor.queue(reactor::SelectEntity {entity});
		
	} else {
		auto [dx, dy] = to_size(delta_x, delta_y);
		const auto old_pos = scene.registry.get<Position>(entity);
		const Position new_pos { old_pos.x + dx, old_pos.y + dy };

		reactor.queue(reactor::MoveEntity {entity, old_pos, new_pos});
	}

	this->in_progress_mouse_data = std::nullopt;

	return true;
}


void SceneView::render(SDL_Renderer* renderer, const model::Scene& scene) {
	SDL_GetRendererOutputSize(renderer, &this->render_w, &this->render_h);

	// Draw scene background
	{
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

		auto [screen_x, screen_y] = to_screen(Position {0.0f, 0.0f});
		auto [screen_w, screen_h] = to_screen(Size {scene.world_size, scene.world_size});

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

		// Draw selection
		if (scene.registry.has<Selected>(entity)) {
			const SDL_Rect draw_rect {
				screen_x-2, screen_y-2,
				screen_w+4, screen_h+4,
			};

			SDL_SetRenderDrawColor(renderer, 230, 30, 30, 255);
			SDL_RenderFillRect(renderer, &draw_rect);
		}

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

	// Draw dragged entity
	if (this->in_progress_mouse_data && this->in_progress_mouse_data->focussed_entity) {
		const auto& [start_x, start_y, delta_x, delta_y, drag_state, entity_maybe] = *this->in_progress_mouse_data;
		auto entity = *entity_maybe;

		auto [position, size, color] = scene.registry.get<Position, Size, Color>(entity);
		auto [dx, dy] = to_size(delta_x, delta_y);

		auto [screen_x, screen_y] = to_screen(Position {position.x + dx, position.y + dy});
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


} // namespace scene_view