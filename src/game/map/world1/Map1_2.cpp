#include "Map1_2.hpp"

#include "../MapLoader.hpp"

Map1_2::Map1_2() {
	mapLoader_->load("assets/maps/1-2.map");
}

Map1_2::~Map1_2() = default;

void Map1_2::onStart() {
}

void Map1_2::onUpdate(float deltaTime) {
}

void Map1_2::onDraw(Renderer& renderer) {
	const float ts = MapLoader::kTileSize;
	for (int row = 0; row < mapLoader_->getHeight(); ++row) {
		for (int col = 0; col < mapLoader_->getWidth(); ++col) {
			if (mapLoader_->getTile(col, row) == TileType::WALL) {
				glm::vec2 pos = { col * ts + ts / 2.0f, row * ts + ts / 2.0f };
				wallBlock_.draw(renderer, pos);
			}
		}
	}
}

void Map1_2::onEnd() {
}
