#pragma once


struct AffectsTerrain {};

struct Position { float x, y; };
struct Size { float w, h; };
struct Color { float r, g, b; };

struct Selected {};

void register_component_typeinfo();