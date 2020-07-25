#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <cstdio>
#include <thread>
#include <chrono>


struct Position { float x, y; };
struct Size { float w, h; };
struct Behaviour { float vx, vy; };


struct Scene {
	entt::registry registry {};
	float world_w {20.0f}, world_h {20.0f};

	void update() {
		// Move
		registry.view<Behaviour, Position>().each(
			[this] (auto& behaviour, auto& position) {
				position.x += behaviour.vx / 60.0f;
				position.y += behaviour.vy / 60.0f;
			}
		);

		// Wrap around scene
		registry.view<Position, const Size>().each(
			[this] (auto& pos, const auto& size) {
				if (pos.x < 0.0f) {
					pos.x = world_w - size.w;
				} else if (pos.x + size.w > world_w) {
					pos.x = 0.0f;
				} 

				if (pos.y < 0.0f) {
					pos.y = world_h - size.h;
				} else if (pos.y + size.h > world_h) {
					pos.y = 0.0f;
				}
			}
		);
	}
};




struct SceneView {
	float camera_x {10.0f}, camera_y {10.0f};
	float camera_scale {20.0f};

	int render_w {}, render_h {};

	std::pair<int, int> to_screen(Position pos) const {
		return {
			int((pos.x - camera_x)*camera_scale + render_w/2.0f),
			int((pos.y - camera_y)*camera_scale + render_h/2.0f)
		};
	}

	std::pair<int, int> to_screen(Size size) const {
		return {int(size.w*camera_scale), int(size.h*camera_scale)};
	}

	void render(SDL_Renderer* renderer, const Scene& scene) {
		SDL_GetRendererOutputSize(renderer, &render_w, &render_h);

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

		SDL_SetRenderDrawColor(renderer, 255, 100, 200, 255);

		auto view = scene.registry.view<const Position, const Size>();
		for (auto entity : view) {
			auto [position, size] = view.get<const Position, const Size>(entity);

			auto [screen_x, screen_y] = to_screen(position);
			auto [screen_w, screen_h] = to_screen(size);

			const SDL_Rect draw_rect {
				screen_x, screen_y,
				screen_w, screen_h,
			};

			SDL_RenderFillRect(renderer, &draw_rect);
		}
	}
};





int main() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::printf("SDL init failed: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"editor",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);
	if (!window) {
		std::printf("Failed to create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::printf("Failed to create renderer: %s\n", SDL_GetError());
		return 1;
	}

	Scene scene {};
	SceneView scene_view {};

	for (int i = 0; i < 10; i++) {
		auto entity = scene.registry.create();
		scene.registry.emplace<Position>(entity, float(i), 10.0f-(i%5)*2.0f);
		scene.registry.emplace<Size>(entity, 2.0f, 2.0f);

		if (i % 2) {
			scene.registry.emplace<Behaviour>(entity, 1.0f, 0.0f);
		}
	}


	bool running = true;
	while (running) {
		SDL_Event e {};
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: running = false;
				default: break;
			}
		}

		scene.update();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		scene_view.render(renderer, scene);

		SDL_RenderPresent(renderer);
	}
}