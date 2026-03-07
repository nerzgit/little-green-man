#include "Map1_1.hpp"

#include "../../player/Player.hpp"
#include "Map1_2.hpp"

Map1_1::Map1_1() {
	loadMap("assets/maps/1-1_地面.csv", TileType::EMPTY);
	loadMap("assets/maps/1-1_障害物.csv", TileType::WALL);
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
