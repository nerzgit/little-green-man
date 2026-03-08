#include "Map1_2.hpp"

Map1_2::Map1_2() {
	// PLAYER_SPAWN を最初のレイヤーにすることで getPlayerSpawnPosition() が正しく動く
	loadMap("assets/maps/1-2_player.csv", TileType::PLAYER_SPAWN,
	        DrawLayer::Background);
	loadMap("assets/maps/1-2_地面.csv", TileType::EMPTY, DrawLayer::Background);
	loadMap("assets/maps/1-2_enemies.csv", TileType::ENEMY_SPAWN,
	        DrawLayer::Background);
	loadMap("assets/maps/1-2_オブジェクト後面.csv", TileType::EMPTY,
	        DrawLayer::Background);
	loadMap("assets/maps/1-2_オブジェクト前面.csv", TileType::EMPTY,
	        DrawLayer::Foreground);
	loadMap("assets/maps/1-2_壁.csv", TileType::WALL, DrawLayer::Foreground);
	loadTileset("assets/maps/[Base]BaseChip_pipo.png");
}

Map1_2::~Map1_2() = default;
