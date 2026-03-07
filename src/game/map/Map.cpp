#include "Map.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../player/Player.hpp"
#include "MapLoader.hpp"

Map::Map() = default;

Map::~Map() = default;

void Map::start() {
	enemies_.clear();
	enemyResolvers_.clear();

	if (mapLoaders_.empty())
		return;

	// 全レイヤーから敵スポーン位置を収集して Enemy を生成
	const float ts          = MapLoader::kTileSize;
	const float stageWidth  = getPixelWidth();
	const float stageHeight = getPixelHeight();
	for (const auto& ml : mapLoaders_) {
		for (const auto& [col, row] : ml->getEnemySpawns()) {
			// 敵の中心座標を計算して Enemy を生成
			enemies_.emplace_back(std::make_unique<Enemy>(
			  glm::vec2(col * ts + ts / 2.0f, row * ts + ts / 2.0f), 150.0f));
			// 敵とマップの衝突解決クラスも生成して保持
			enemyResolvers_.emplace_back(std::make_unique<CollisionResolver>(
			  *enemies_.back(), *this, stageWidth, stageHeight));
		}
	}
}

MapEvent Map::update(float deltaTime, Player& player) {
	for (size_t i = 0; i < enemies_.size(); ++i) {
		enemies_[i]->update(deltaTime);
		enemyResolvers_[i]->resolve();
		if (player.intersects(*enemies_[i])) {
			return MapEvent::PlayerDead;
		}
	}
	return MapEvent::None;
}

void Map::loadMap(const std::string& csvPath, TileType tileType) {
	auto ml = std::make_unique<MapLoader>();
	ml->loadMap(csvPath, tileType);
	mapLoaders_.push_back(std::move(ml));
}

void Map::loadTileset(const std::string& tilesetPath) {
	tileset_ = std::make_unique<TilesetLoader>(tilesetPath);
}

void Map::draw(Renderer& renderer) {
	// 全レイヤーをタイルセットで描画
	if (!tileset_) {
		throw std::runtime_error("Failed to draw map: Tileset not loaded");
	}

	// タイルセットでマップを描画
	for (const auto& ml : mapLoaders_) {
		tileset_->draw(renderer, *ml, MapLoader::kTileSize);
	}

	// 敵の描画
	for (auto& enemy : enemies_) {
		enemy->draw(renderer);
	}
}

bool Map::isWallAt(float x, float y) const {
	for (const auto& ml : mapLoaders_) {
		if (ml->isWallAt(x, y))
			return true;
	}
	return false;
}

bool Map::isTriggerAt(float x, float y) const {
	for (const auto& ml : mapLoaders_) {
		if (ml->isTriggerAt(x, y))
			return true;
	}
	return false;
}

glm::vec2 Map::getPlayerSpawnPosition() const {
	if (mapLoaders_.empty())
		return {};
	const float ts  = MapLoader::kTileSize;
	const float col = static_cast<float>(mapLoaders_[0]->getPlayerSpawnCol());
	const float row = static_cast<float>(mapLoaders_[0]->getPlayerSpawnRow());
	return {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
}

float Map::getPixelWidth() const {
	return mapLoaders_.empty() ?
	         0.0f :
	         mapLoaders_[0]->getWidth() * MapLoader::kTileSize;
}

float Map::getPixelHeight() const {
	return mapLoaders_.empty() ?
	         0.0f :
	         mapLoaders_[0]->getHeight() * MapLoader::kTileSize;
}
