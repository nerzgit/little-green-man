#include "Map1_1.hpp"

#include "Map1_2.hpp"

Map1_1::Map1_1() {
	loadMap("assets/maps/1-1_地面.csv", TileType::EMPTY, DrawLayer::Background);
	loadMap("assets/maps/1-1_オブジェクト後面.csv", TileType::EMPTY,
	        DrawLayer::Background);
	loadMap("assets/maps/1-1_オブジェクト前面.csv", TileType::EMPTY,
	        DrawLayer::Foreground);
	loadMap("assets/maps/1-1_壁.csv", TileType::WALL, DrawLayer::Foreground);
	loadTileset("assets/maps/[Base]BaseChip_pipo.png");
}

Map1_1::~Map1_1() = default;

std::unique_ptr<Map> Map1_1::createNextMap() {
	return std::make_unique<Map1_2>();
}
