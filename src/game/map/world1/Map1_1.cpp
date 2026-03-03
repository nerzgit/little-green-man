#include "Map1_1.hpp"

#include "../MapLoader.hpp"

Map1_1::Map1_1() {
	mapLoader_->load("assets/maps/1-1.map");
}

Map1_1::~Map1_1() = default;

void Map1_1::onStart() {
}

void Map1_1::onUpdate(float deltaTime) {
}

void Map1_1::onDraw(Renderer& renderer) {
	const float ts = MapLoader::kTileSize;
	for (int row = 0; row < mapLoader_->getHeight(); ++row) {
		for (int col = 0; col < mapLoader_->getWidth(); ++col) {
			if (mapLoader_->getTile(col, row) == TileType::WALL) {
				glm::vec2 pos = {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
				wallBlock_.draw(renderer, pos);
			}
		}
	}
}

void Map1_1::onEnd() {
	// TODO: ステージ終了後の処理
}
