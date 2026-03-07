#include "Map.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../player/Player.hpp"
#include "MapLoader.hpp"

Map::Map() : mapLoader_(std::make_unique<MapLoader>()) {
}

Map::~Map() = default;

void Map::start() {
	// 敵の初期化
	enemies_.clear();
	enemyResolvers_.clear();

	// マップローダーから敵のスポーン位置を取得して Enemy を生成
	const float ts          = MapLoader::kTileSize;
	const float stageWidth  = getPixelWidth();
	const float stageHeight = getPixelHeight();
	for (const auto& [col, row] : mapLoader_->getEnemySpawns()) {
		enemies_.emplace_back(std::make_unique<Enemy>(
		  glm::vec2(col * ts + ts / 2.0f, row * ts + ts / 2.0f), 150.0f));
		enemyResolvers_.emplace_back(std::make_unique<CollisionResolver>(
		  *enemies_.back(), *this, stageWidth, stageHeight));
	}
}

MapEvent Map::update(float deltaTime, Player& player) {
	// 敵の更新とプレイヤーとの当たり判定
	for (size_t i = 0; i < enemies_.size(); ++i) {
		enemies_[i]->update(deltaTime);
		enemyResolvers_[i]->resolve();
		if (player.intersects(*enemies_[i])) {
			return MapEvent::PlayerDead;
		}
	}
	return MapEvent::None;
}

void Map::draw(Renderer& renderer) {
	// タイルの描画
	const float ts = MapLoader::kTileSize;
	for (int row = 0; row < mapLoader_->getHeight(); ++row) {
		for (int col = 0; col < mapLoader_->getWidth(); ++col) {
			if (mapLoader_->getTile(col, row) == TileType::WALL) {
				glm::vec2 pos = {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
				wallBlock_.draw(renderer, pos);
			}
		}
	}

	// 敵の描画
	for (auto& enemy : enemies_) {
		enemy->draw(renderer);
	}
}

bool Map::isWallAt(float x, float y) const {
	return mapLoader_->isWallAt(x, y);
}

bool Map::isTriggerAt(float x, float y) const {
	return mapLoader_->isTriggerAt(x, y);
}

glm::vec2 Map::getPlayerSpawnPosition() const {
	const float ts  = MapLoader::kTileSize;
	const float col = static_cast<float>(mapLoader_->getPlayerSpawnCol());
	const float row = static_cast<float>(mapLoader_->getPlayerSpawnRow());
	return {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
}

float Map::getPixelWidth() const {
	return mapLoader_->getWidth() * MapLoader::kTileSize;
}

float Map::getPixelHeight() const {
	return mapLoader_->getHeight() * MapLoader::kTileSize;
}
