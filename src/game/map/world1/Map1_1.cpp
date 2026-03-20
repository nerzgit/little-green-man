#include "Map1_1.hpp"

#include "../../light/LightSystem.hpp"
#include "Map1_2.hpp"

Map1_1::Map1_1() {
	loadMap("assets/maps/1-3_床.csv", TileType::EMPTY, DrawLayer::Background);
	loadMap("assets/maps/1-3_壁.csv", TileType::WALL, DrawLayer::Background);
	loadMap("assets/maps/1-3_オブジェクト後面.csv", TileType::EMPTY,
	        DrawLayer::Background);
	loadMap("assets/maps/1-2_player.csv", TileType::PLAYER_SPAWN,
	        DrawLayer::Foreground);
	loadMap("assets/maps/1-3_オブジェクト.csv", TileType::EMPTY,
	        DrawLayer::Foreground);
	loadMap("assets/maps/1-3_オブジェクト2.csv", TileType::EMPTY,
	        DrawLayer::Foreground);
	loadMap("assets/maps/1-3_壺.csv", TileType::EMPTY, DrawLayer::Foreground);
	loadMap("assets/maps/1-3_ゴール.csv", TileType::EMPTY,
	        DrawLayer::Background);
	loadTileset("assets/maps/merged.png");
	// lightSystem_->loadTileset("assets/maps/[Base]BaseChip_pipo.png");
	// lightSystem_->loadLightSources("assets/maps/1-1_光源.csv");
	// lightSystem_->loadMirrors("assets/maps/1-1_鏡.csv");
	// lightSystem_->loadReceivers("assets/maps/1-1_受信機.csv");
	// lightSystem_->setOverlayColor(0, {0.0f, 0.0f, 0.0f, 0.5f});
	// lightSystem_->setOverlayColor(1, {0.0f, 0.0f, 0.0f, 0.1f});
	colorOverlay_.color    = {158 / 255.0f, 139 / 255.0f, 81 / 255.0f, 0.1f};
	parallaxOverlay_.speed = 1.4f;
	parallaxOverlay_.load("assets/maps/pixelparallax.png");
}

Map1_1::~Map1_1() = default;

std::unique_ptr<Map> Map1_1::createNextMap() {
	return std::make_unique<Map1_2>();
}
