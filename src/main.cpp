#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <cstdio>
#include <thread>
#include <chrono>

#include "common.h"
#include "components.h"
#include "reactor.h"

#include "model/scene.h"
#include "view/scene_view.h"

#include "model/terrain.h"
#include "view/terrain_view.h"

// https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system
// https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between



int main() {
	register_component_typeinfo();

	// Set stdout to use line buffering
	std::setvbuf(stdout, nullptr, _IOLBF, 0);

	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::printf("SDL init failed: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"editor",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_RESIZABLE
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

	// Model
	model::Scene scene {};
	model::Terrain terrain {scene, 30};

	for (int i = 0; i < 10; i++) {
		auto entity = scene.registry.create();
		scene.registry.emplace<Position>(entity, randf(0.0f, scene.world_size), randf(0.0f, scene.world_size));
		scene.registry.emplace<Size>(entity, 2.0f, 2.0f);
		scene.registry.emplace<Color>(entity, randf(0.5f, 1.0f), randf(0.5f, 1.0f), randf(0.5f, 1.0f));

		if (i&2) {
			scene.registry.emplace<AffectsTerrain>(entity);
		}
	}

	std::printf("Component ids: ");
	scene.registry.visit([] (const auto component) {
		auto display_prop = entt::resolve_type(component).prop("display"_hs);
		if (!display_prop) {
			std::printf("<unknown> ");
		} else {
			std::printf("%s ", display_prop.value().template cast<const char*>());
		}

	});
	std::puts("");


	// View
	view::scene::SceneView scene_view {};
	view::terrain::TerrainView terrain_view {renderer, terrain};


	reactor::Reactor reactor {};


	bool running = true;
	while (running) {
		SDL_Event e {};
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: running = false; break;
				case SDL_MOUSEBUTTONDOWN: {
					if (e.button.button != SDL_BUTTON_LEFT) {
						break;
					}

					const auto mx = e.button.x;
					const auto my = e.button.y;

					if (scene_view.handle_mouse_down(mx, my, scene, reactor)) {
						break;
					}

					break;
				}
				case SDL_MOUSEMOTION: {
					if (e.motion.state != SDL_BUTTON_LMASK) {
						break;
					}

					const auto mx = e.motion.x;
					const auto my = e.motion.y;

					if (scene_view.handle_mouse_move(mx, my, scene, reactor)) {
						break;
					}

					break;
				}
				case SDL_MOUSEBUTTONUP: {
					if (e.button.button != SDL_BUTTON_LEFT) {
						break;
					}

					const auto mx = e.button.x;
					const auto my = e.button.y;

					if (scene_view.handle_mouse_up(mx, my, scene, reactor)) {
						break;
					}

					break;
				}

				default: break;
			}
		}

		reactor.react(scene, terrain);

		scene.update();
		terrain.update(scene);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		scene_view.render(renderer, scene);
		terrain_view.render(renderer, terrain);

		SDL_RenderPresent(renderer);
	}
}