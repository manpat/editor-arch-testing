#pragma once

#include <cstdint>
#include <memory>

struct SDL_Renderer;
struct SDL_Texture;
struct Color;

namespace model {
	struct Terrain;
}

namespace view::terrain {
	struct TerrainView {
		TerrainView(SDL_Renderer*, const model::Terrain&);
		void render(SDL_Renderer*, const model::Terrain&);

	private:
		void plot(int x, int y, Color);

	private:
		SDL_Texture* texture;
		std::unique_ptr<uint8_t[]> texture_data;
		int resolution;
	};
}