#include "view/terrain_view.h"
#include "model/terrain.h"
#include "components.h"

#include <SDL2/SDL.h>


namespace view::terrain {


TerrainView::TerrainView(SDL_Renderer* renderer, const model::Terrain& terrain)
	: texture_data{std::make_unique<uint8_t[]>(terrain.resolution * terrain.resolution * 4)}
	, resolution{terrain.resolution}
{
	this->texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 
		this->resolution, this->resolution
	);
}


void TerrainView::render(SDL_Renderer* renderer, const model::Terrain& terrain) {
	for (int y = 0; y < this->resolution; y++) {
		for (int x = 0; x < this->resolution; x++) {
			const auto value = terrain.get(x, y);
			plot(x, y, Color {0.2f + value*0.8f, 0.2f, 0.2f});
		}
	}

	const SDL_Rect dest_rect {
		0, 0,
		200, 200,
	};
	
	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(this->texture, nullptr, this->texture_data.get(), this->resolution*4);
	SDL_RenderCopy(renderer, this->texture, nullptr, &dest_rect);
}


void TerrainView::plot(int x, int y, Color color) {
	if (x < 0 || x >= this->resolution) { return; }
	if (y < 0 || y >= this->resolution) { return; }

	const auto start_index = (x + y * this->resolution) * 4;
	this->texture_data[start_index+0] = uint8_t(color.r*255.0f);
	this->texture_data[start_index+1] = uint8_t(color.g*255.0f);
	this->texture_data[start_index+2] = uint8_t(color.b*255.0f);
	this->texture_data[start_index+3] = 180;
}


} // namespace view::terrain