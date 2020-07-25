#pragma once

#include <utility>
#include <optional>
#include <entt/entity/entity.hpp>

struct SDL_Renderer;

struct Position;
struct Size;
struct Scene;

namespace scene_view {
	enum class DragState {
		Stationary,
		Dragging,
	};


	struct InProgressMouseData {
		int start_x, start_y;
		int delta_x, delta_y;
		DragState drag_state;

		std::optional<entt::entity> focussed_entity;
	};


	struct SceneView {
		std::pair<int, int> to_screen(Position) const;
		std::pair<int, int> to_screen(Size) const;
		Position to_position(int x, int y) const;
		Size to_size(int x, int y) const;

		bool handle_mouse_down(int sx, int sy, const Scene&);
		bool handle_mouse_move(int sx, int sy, const Scene&);
		bool handle_mouse_up(int sx, int sy, Scene&);

		void render(SDL_Renderer*, const Scene&);

	public:
		float camera_x {10.0f}, camera_y {10.0f};
		float camera_scale {20.0f};

	private:
		int render_w {}, render_h {};
		
		std::optional<InProgressMouseData> in_progress_mouse_data {};
	};
}




