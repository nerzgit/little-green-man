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
	loadLightTileset("assets/maps/[Base]BaseChip_pipo.png");
	loadLightSources("assets/maps/1-1_光源.csv");
	loadMirrors("assets/maps/1-1_鏡.csv");
	loadReceivers("assets/maps/1-1_受信機.csv");
	setLightOverlayColor(0, {0.0f, 0.0f, 0.0f, 0.5f});
	setLightOverlayColor(1, {0.0f, 0.0f, 0.0f, 0.1f});
	colorOverlay_.color = {158 / 255.0f, 139 / 255.0f, 81 / 255.0f, 0.1f};
	loadParallaxOverlay("assets/maps/pixelparallax.png", 0.7f);
}

Map1_1::~Map1_1() = default;

std::unique_ptr<Map> Map1_1::createNextMap() {
	return std::make_unique<Map1_2>();
}
