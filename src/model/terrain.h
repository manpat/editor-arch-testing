#pragma once

#include <memory>

struct Position;
struct Size;

namespace model {
	struct Scene;
}

namespace model {
	struct Terrain {
		Terrain(model::Scene&, int resolution);

		void mark_dirty(Position, Size);
		void update(const model::Scene&);

		float get(int x, int y) const;

	public:
		int resolution;

	private:
		std::unique_ptr<float[]> data;
		float scene_to_data {1.0f};
		bool dirty {true};
	};
}
