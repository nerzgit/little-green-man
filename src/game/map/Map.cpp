#include "Map.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../GameConstants.hpp"
#include "../enemy/EnemySystem.hpp"
#include "../player/Player.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

// -----------------------------------------------------------------------------
//  JSON ファクトリ
// -----------------------------------------------------------------------------

static TileType parseTileType(const std::string& s) {
	if (s == "wall")         return TileType::WALL;
	if (s == "player_spawn") return TileType::PLAYER_SPAWN;
	if (s == "enemy_spawn")  return TileType::ENEMY_SPAWN;
	return TileType::EMPTY;
}

static DrawLayer parseDrawLayer(const std::string& s) {
	if (s == "foreground") return DrawLayer::Foreground;
	return DrawLayer::Background;
}

std::unique_ptr<Map> Map::load(const std::string& jsonPath) {
	std::ifstream f(jsonPath);
	if (!f.is_open())
		throw std::runtime_error("Map file not found: " + jsonPath);

	const json root = json::parse(f);
	auto map = std::make_unique<Map>();

	for (const auto& layer : root.at("layers")) {
		map->loadLayer(
		  layer.at("csv").get<std::string>(),
		  parseTileType(layer.at("type").get<std::string>()),
		  parseDrawLayer(layer.at("draw").get<std::string>()));
	}

	map->loadTileset(root.at("tileset").get<std::string>());
	map->nextMapPath_ = root.value("next_map", "");
	map->storyPath_   = root.value("story",    "");

	return map;
}

// -----------------------------------------------------------------------------

Map::Map()
    : enemySystem_(std::make_unique<EnemySystem>()) {
}

Map::~Map() = default;

void Map::start() {
	enemySystem_->spawnFrom(getEnemySpawnPositions(), *this);
}

MapEvent Map::update(float deltaTime, Player& player) {
	if (enemySystem_->update(deltaTime, player))
		return MapEvent::PlayerDead;
	return MapEvent::None;
}

// -----------------------------------------------------------------------------
//  タイルレイヤー管理
// -----------------------------------------------------------------------------

void Map::loadLayer(const std::string& csvPath, TileType defaultType,
                    DrawLayer drawLayer) {
	auto loader = std::make_unique<MapLoader>();
	loader->loadMap(csvPath, defaultType);
	layers_.push_back({std::move(loader), drawLayer});
}

void Map::loadTileset(const std::string& tilesetPath) {
	tileset_ = std::make_unique<TilesetLoader>(tilesetPath);
}

void Map::draw(Renderer& renderer, DrawLayer layer) {
	if (!tileset_)
		throw std::runtime_error("Tileset not loaded");
	for (const auto& l : layers_) {
		if (l.drawLayer != layer)
			continue;
		tileset_->draw(renderer, *l.loader, GameConstants::kTileSize);
	}
}

// -----------------------------------------------------------------------------
//  タイル判定
// -----------------------------------------------------------------------------

bool Map::isWallAt(float x, float y) const {
	for (const auto& l : layers_) {
		if (l.loader->isWallAt(x, y))
			return true;
	}
	return false;
}

bool Map::isTriggerAt(float x, float y) const {
	for (const auto& l : layers_) {
		if (l.loader->isTriggerAt(x, y))
			return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
//  マップサイズ・スポーン位置
// -----------------------------------------------------------------------------

float Map::getPixelWidth() const {
	return layers_.empty() ? 0.0f :
	       layers_[0].loader->getWidth() * GameConstants::kTileSize;
}

float Map::getPixelHeight() const {
	return layers_.empty() ? 0.0f :
	       layers_[0].loader->getHeight() * GameConstants::kTileSize;
}

glm::vec2 Map::getPlayerSpawnPosition() const {
	const float ts = GameConstants::kTileSize;
	for (const auto& l : layers_) {
		if (l.loader->hasPlayerSpawn()) {
			const float col = static_cast<float>(l.loader->getPlayerSpawnCol());
			const float row = static_cast<float>(l.loader->getPlayerSpawnRow());
			return {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
		}
	}
	return {};
}

std::vector<glm::vec2> Map::getEnemySpawnPositions() const {
	const float            ts = GameConstants::kTileSize;
	std::vector<glm::vec2> positions;
	for (const auto& l : layers_) {
		for (const auto& [col, row] : l.loader->getEnemySpawns()) {
			positions.emplace_back(col * ts + ts / 2.0f, row * ts + ts / 2.0f);
		}
	}
	return positions;
}

// -----------------------------------------------------------------------------

const EnemySystem& Map::enemySystem() const { return *enemySystem_; }

std::unique_ptr<Map> Map::createNextMap() const {
	if (nextMapPath_.empty())
		return nullptr;
	return Map::load(nextMapPath_);
}

const std::string& Map::getStoryPath() const { return storyPath_; }
