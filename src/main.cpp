#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <cstdio>
#include <thread>
#include <chrono>

#include "common.h"
#include "components.h"

#include "scene.h"
#include "view/scene_view.h"

// https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system
// https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between



int main() {
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
	Scene scene {};

	for (int i = 0; i < 10; i++) {
		auto entity = scene.registry.create();
		scene.registry.emplace<Position>(entity, randf(0.0f, scene.world_w), randf(0.0f, scene.world_h));
		scene.registry.emplace<Size>(entity, 2.0f, 2.0f);
		scene.registry.emplace<Color>(entity, randf(0.5f, 1.0f), randf(0.5f, 1.0f), randf(0.5f, 1.0f));
		scene.registry.emplace<AffectsTerrain>(entity);
	}


	// View
	SceneView scene_view {};


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