#include "MapLoader.hpp"

#include "../GameConstants.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

MapLoader::MapLoader() = default;

MapLoader::~MapLoader() = default;

void MapLoader::loadMap(const std::string& path, TileType tileType) {
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open map file: " + path);
	}

	tiles_.clear();
	width_  = 0;
	height_ = 0;
	enemySpawns_.clear();

	std::string line;
	int         row = 0;
	while (std::getline(file, line)) {
		std::vector<Tile> rowTiles;
		std::stringstream ss(line);
		std::string       cell;
		int               col = 0;
		while (std::getline(ss, cell, ',')) {
			int tyleId = std::stoi(cell);

			if (tyleId > -1 && tileType == TileType::PLAYER_SPAWN) {
				playerSpawnCol_ = col;
				playerSpawnRow_ = row;
			} else if (tyleId > -1 && tileType == TileType::ENEMY_SPAWN) {
				enemySpawns_.emplace_back(col, row);
			} else {
				// タイルIDが -1 の場合は空タイルとみなす
				if (tyleId == -1) {
					rowTiles.push_back({tyleId, TileType::EMPTY});
				} else {
					rowTiles.push_back({tyleId, tileType});
				}
			}

			++col;
		}
		tiles_.push_back(std::move(rowTiles));
		width_ = std::max(width_, col);
		++row;
	}
	height_ = row;
}

Tile MapLoader::getTile(int col, int row) const {
	if (row < 0 || row >= height_ || col < 0
	    || col >= static_cast<int>(tiles_[row].size())) {
		return {0, TileType::EMPTY};
	}
	return tiles_[row][col];
}

TileType MapLoader::getTileType(int col, int row) const {
	return getTile(col, row).type;
}

bool MapLoader::isWallAt(float x, float y) const {
	int col = static_cast<int>(x / GameConstants::kTileSize);
	int row = static_cast<int>(y / GameConstants::kTileSize);
	return getTile(col, row).type == TileType::WALL;
}

bool MapLoader::isTriggerAt(float x, float y) const {
	int col = static_cast<int>(x / GameConstants::kTileSize);
	int row = static_cast<int>(y / GameConstants::kTileSize);
	return getTile(col, row).type == TileType::TRIGGER;
}