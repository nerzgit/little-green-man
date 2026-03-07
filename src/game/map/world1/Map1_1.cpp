#include "Map1_1.hpp"

#include "../../player/Player.hpp"
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

MapEvent Map1_1::update(float deltaTime, Player& player) {
	auto event = Map::update(deltaTime, player);
	if (event != MapEvent::None)
		return event;

	if (isTriggerAt(player.position.x, player.position.y)) {
		return MapEvent::NextMap;
	}
	return MapEvent::None;
}

std::unique_ptr<Map> Map1_1::createNextMap() {
	return std::make_unique<Map1_2>();
}
