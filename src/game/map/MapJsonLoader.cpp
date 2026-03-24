#include "Map.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

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
		map->layers_.emplace_back(
		  layer.at("csv").get<std::string>(),
		  parseTileType(layer.at("type").get<std::string>()),
		  parseDrawLayer(layer.at("draw").get<std::string>()));
	}

	map->loadTileset(root.at("tileset").get<std::string>());
	map->nextMapPath_ = root.value("next_map", "");
	map->storyPath_   = root.value("story",    "");

	return map;
}
