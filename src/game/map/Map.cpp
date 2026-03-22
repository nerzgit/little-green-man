#include "Map.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../GameConstants.hpp"
#include "../enemy/EnemySystem.hpp"
#include "../light/LightSystem.hpp"
#include "../player/Player.hpp"
#include "MapLoader.hpp"
#include <memory>

Map::Map()
    : lightSystem_(std::make_unique<LightSystem>())
    , enemySystem_(std::make_unique<EnemySystem>()) {
	drawableSources_ = {enemySystem_.get(), lightSystem_.get()};
};

Map::~Map() = default;

void Map::start() {
	if (mapLayers_.empty())
		return;

	const float ts          = GameConstants::kTileSize;
	const float stageWidth  = getPixelWidth();
	const float stageHeight = getPixelHeight();

	std::vector<glm::vec2> spawnPositions;
	for (const auto& layer : mapLayers_) {
		for (const auto& [col, row] : layer.loader->getEnemySpawns()) {
			spawnPositions.emplace_back(col * ts + ts / 2.0f,
			                            row * ts + ts / 2.0f);
		}
	}
	enemySystem_->spawnFrom(spawnPositions, stageWidth, stageHeight, *this);
}

MapEvent Map::update(float deltaTime, Player& player) {
	if (enemySystem_->update(deltaTime, player))
		return MapEvent::PlayerDead;
	lightSystem_->update(deltaTime, *this);
	lightSystem_->trace(*this);
	return MapEvent::None;
}

void Map::loadMap(const std::string& csvPath,
                  TileType           defaultType,
                  DrawLayer          drawLayer) {
	auto ml = std::make_unique<MapLoader>();
	ml->loadMap(csvPath, defaultType);
	mapLayers_.push_back({std::move(ml), drawLayer});
}

void Map::loadTileset(const std::string& tilesetPath) {
	tileset_ = std::make_unique<TilesetLoader>(tilesetPath);
}

int Map::getActivatedReceiverCount() const {
	return lightSystem_->getActivatedReceiverCount();
}

void Map::drawEffects(Renderer& renderer, int windowWidth, int windowHeight,
                      float cameraX, float playerY) const {
	lightSystem_->draw(renderer);
	lightSystem_->drawOverlay(renderer, windowWidth, windowHeight);
	parallaxOverlay_.draw(renderer, windowWidth, windowHeight, cameraX, playerY);
	colorOverlay_.draw(renderer, windowWidth, windowHeight);
}

void Map::collectDrawables(std::vector<Drawable>& out) {
	for (auto* src : drawableSources_)
		src->collectDrawables(out);
}

void Map::draw(Renderer& renderer, DrawLayer layer) {
	if (!tileset_) {
		throw std::runtime_error("Tileset not loaded");
	}
	for (const auto& ml : mapLayers_) {
		if (ml.drawLayer != layer)
			continue;
		tileset_->draw(renderer, *ml.loader, GameConstants::kTileSize);
	}
}

bool Map::isMirrorAt(float x, float y) const {
	return lightSystem_->isMirrorAt({x, y});
}

void Map::resolvePush(Entity& pusher) {
	const float ts = GameConstants::kTileSize;
	const float r  = pusher.size / 2.0f;
	glm::vec2&  pos = pusher.position;

	// 左エッジがミラーに入ったら左に押し出し試行
	if (isMirrorAt(pos.x - r, pos.y)) {
		if (!lightSystem_->tryPush({pos.x - r, pos.y}, {-1, 0}, *this)) {
			int col = static_cast<int>((pos.x - r) / ts);
			pos.x   = (col + 1) * ts + r;
		}
	}
	// 右エッジ
	if (isMirrorAt(pos.x + r, pos.y)) {
		if (!lightSystem_->tryPush({pos.x + r, pos.y}, {1, 0}, *this)) {
			int col = static_cast<int>((pos.x + r) / ts);
			pos.x   = col * ts - r;
		}
	}
	// 上エッジ
	if (isMirrorAt(pos.x, pos.y - r)) {
		if (!lightSystem_->tryPush({pos.x, pos.y - r}, {0, -1}, *this)) {
			int row = static_cast<int>((pos.y - r) / ts);
			pos.y   = (row + 1) * ts + r;
		}
	}
	// 下エッジ
	if (isMirrorAt(pos.x, pos.y + r)) {
		if (!lightSystem_->tryPush({pos.x, pos.y + r}, {0, 1}, *this)) {
			int row = static_cast<int>((pos.y + r) / ts);
			pos.y   = row * ts - r;
		}
	}
}

bool Map::isWallAt(float x, float y) const {
	for (const auto& layer : mapLayers_) {
		if (layer.loader->isWallAt(x, y))
			return true;
	}
	return false;
}

bool Map::isTriggerAt(float x, float y) const {
	for (const auto& layer : mapLayers_) {
		if (layer.loader->isTriggerAt(x, y))
			return true;
	}
	return false;
}

glm::vec2 Map::getPlayerSpawnPosition() const {
	const float ts = GameConstants::kTileSize;
	for (const auto& ml : mapLayers_) {
		if (ml.loader->hasPlayerSpawn()) {
			const float col = static_cast<float>(ml.loader->getPlayerSpawnCol());
			const float row = static_cast<float>(ml.loader->getPlayerSpawnRow());
			return {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
		}
	}
	return {};
}

float Map::getPixelWidth() const {
	return mapLayers_.empty() ?
	         0.0f :
	         mapLayers_[0].loader->getWidth() * GameConstants::kTileSize;
}

float Map::getPixelHeight() const {
	return mapLayers_.empty() ?
	         0.0f :
	         mapLayers_[0].loader->getHeight() * GameConstants::kTileSize;
}
