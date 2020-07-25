#pragma once

#include <utility>

struct SDL_Renderer;

struct Position;
struct Size;
struct Scene;

struct SceneView {
	float camera_x {10.0f}, camera_y {10.0f};
	float camera_scale {20.0f};

	int render_w {}, render_h {};

	std::pair<int, int> to_screen(Position pos) const;
	std::pair<int, int> to_screen(Size size) const;

	void render(SDL_Renderer* renderer, const Scene& scene);
};



