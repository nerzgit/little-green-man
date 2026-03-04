#include "MapLoader.hpp"

#include <cstdio>
#include <fstream>
#include <stdexcept>

MapLoader::MapLoader() = default;

MapLoader::~MapLoader() = default;

void MapLoader::load(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open map file: " + path);
	}

	tiles_.clear();
	enemySpawns_.clear();
	width_          = 0;
	height_         = 0;
	playerSpawnCol_ = 0;
	playerSpawnRow_ = 0;

	std::string line;
	int         row = 0;
	while (std::getline(file, line)) {
		std::vector<TileType> tileRow;
		for (int col = 0; col < static_cast<int>(line.size()); ++col) {
			TileType tile;
			switch (line[col]) {
			case '#': tile = TileType::WALL; break;
			case 'P':
				tile            = TileType::PLAYER_SPAWN;
				playerSpawnCol_ = col;
				playerSpawnRow_ = row;
				break;
			case 'E':
				tile = TileType::ENEMY_SPAWN;
				enemySpawns_.emplace_back(col, row);
				break;
			case 'T': tile = TileType::TRIGGER; break;
			default: tile = TileType::EMPTY; break;
			}
			tileRow.push_back(tile);
		}
		if (width_ == 0)
			width_ = static_cast<int>(tileRow.size());
		tiles_.push_back(std::move(tileRow));
		++row;
	}
	height_ = row;
}

TileType MapLoader::getTile(int col, int row) const {
	if (row < 0 || row >= height_ || col < 0
	    || col >= static_cast<int>(tiles_[row].size())) {
		return TileType::EMPTY;
	}
	return tiles_[row][col];
}

bool MapLoader::isWallAt(float x, float y) const {
	int col = static_cast<int>(x / kTileSize);
	int row = static_cast<int>(y / kTileSize);
	return getTile(col, row) == TileType::WALL;
}

bool MapLoader::isTriggerAt(float x, float y) const {
	int col = static_cast<int>(x / kTileSize);
	int row = static_cast<int>(y / kTileSize);
	return getTile(col, row) == TileType::TRIGGER;
}