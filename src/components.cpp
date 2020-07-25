#include "components.h"

#include <entt/entt.hpp>

void register_component_typeinfo() {
	entt::meta<Position>().type().prop("display"_hs, "Position");
	entt::meta<Size>().type().prop("display"_hs, "Size");
	entt::meta<Color>().type().prop("display"_hs, "Color");
	entt::meta<AffectsTerrain>().type().prop("display"_hs, "AffectsTerrain");
}
