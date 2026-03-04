#include "Map1_2.hpp"

#include "../MapLoader.hpp"

Map1_2::Map1_2() {
	mapLoader_->load("assets/maps/1-2.map");
}

Map1_2::~Map1_2() = default;
