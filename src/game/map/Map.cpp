#include "Map.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../light/LightSystem.hpp"
#include "../player/Player.hpp"
#include "MapLoader.hpp"
#include <memory>

Map::Map() : lightSystem_(std::make_unique<LightSystem>()) {};

Map::~Map() = default;

void Map::start() {
	enemies_.clear();
	enemyResolvers_.clear();

	if (mapLayers_.empty())
		return;

	const float ts          = MapLoader::kTileSize;
	const float stageWidth  = getPixelWidth();
	const float stageHeight = getPixelHeight();
	for (const auto& layer : mapLayers_) {
		for (const auto& [col, row] : layer.loader->getEnemySpawns()) {
			enemies_.emplace_back(std::make_unique<Enemy>(
			  glm::vec2(col * ts + ts / 2.0f, row * ts + ts / 2.0f), 150.0f));
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

void Map::setLightOverlayColor(int index, const glm::vec4& rgba) {
	lightSystem_->setOverlayColor(index, rgba);
}

void Map::drawLightOverlay(Renderer& renderer, int windowWidth,
                           int windowHeight) const {
	lightSystem_->drawOverlay(renderer, windowWidth, windowHeight);
}

void Map::drawColorOverlay(Renderer& renderer, int windowWidth,
                           int windowHeight) const {
	colorOverlay_.draw(renderer, windowWidth, windowHeight);
}

void Map::loadParallaxOverlay(const std::string& path, float speed) {
	parallaxOverlay_.speed = speed;
	parallaxOverlay_.load(path);
}

void Map::drawParallaxOverlay(Renderer& renderer, int windowWidth,
                              int windowHeight, float playerX) const {
	parallaxOverlay_.draw(renderer, windowWidth, windowHeight, playerX);
}

void Map::loadLightTileset(const std::string& path) {
	lightSystem_->loadTileset(path);
}

void Map::loadLightSources(const std::string& path) {
	lightSystem_->loadLightSources(path);
}

void Map::loadMirrors(const std::string& path) {
	lightSystem_->loadMirrors(path);
}

void Map::loadReceivers(const std::string& path) {
	lightSystem_->loadReceivers(path);
}

void Map::drawEnemy(Renderer& renderer) {
	for (auto& enemy : enemies_) {
		enemy->draw(renderer);
	}
}

void Map::drawLight(Renderer& renderer) {
	lightSystem_->draw(renderer);
}

void Map::drawBackground(Renderer& renderer) {
	if (!tileset_) {
		throw std::runtime_error(
		  "Failed to draw background map: Tileset not loaded");
	}
	for (const auto& layer : mapLayers_) {
		if (layer.drawLayer != DrawLayer::Background)
			continue;
		tileset_->draw(renderer, *layer.loader, MapLoader::kTileSize);
	}
}

void Map::drawForeground(Renderer& renderer) {
	if (!tileset_) {
		throw std::runtime_error(
		  "Failed to draw foreground map: Tileset not loaded");
	}
	for (const auto& layer : mapLayers_) {
		if (layer.drawLayer != DrawLayer::Foreground)
			continue;
		tileset_->draw(renderer, *layer.loader, MapLoader::kTileSize);
	}
}

bool Map::isMirrorAt(float x, float y) const {
	return lightSystem_->isMirrorAt({x, y});
}

void Map::resolvePush(Entity& pusher) {
	const float ts = MapLoader::kTileSize;
	const float r  = pusher.size;
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
	if (mapLayers_.empty())
		return {};
	const float ts = MapLoader::kTileSize;
	const float col =
	  static_cast<float>(mapLayers_[0].loader->getPlayerSpawnCol());
	const float row =
	  static_cast<float>(mapLayers_[0].loader->getPlayerSpawnRow());
	return {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
}

float Map::getPixelWidth() const {
	return mapLayers_.empty() ?
	         0.0f :
	         mapLayers_[0].loader->getWidth() * MapLoader::kTileSize;
}

float Map::getPixelHeight() const {
	return mapLayers_.empty() ?
	         0.0f :
	         mapLayers_[0].loader->getHeight() * MapLoader::kTileSize;
}
