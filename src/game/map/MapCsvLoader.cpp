#include "MapCsvLoader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

void MapCsvLoader::loadMap(const std::string& path, TileType tileType) {
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open map file: " + path);

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
			int tileId = std::stoi(cell);

			if (tileId > -1 && tileType == TileType::PLAYER_SPAWN) {
				playerSpawnCol_ = col;
				playerSpawnRow_ = row;
				hasPlayerSpawn_ = true;
			} else if (tileId > -1 && tileType == TileType::ENEMY_SPAWN) {
				enemySpawns_.emplace_back(col, row);
			} else {
				if (tileId == -1)
					rowTiles.push_back({tileId, TileType::EMPTY});
				else
					rowTiles.push_back({tileId, tileType});
			}
			++col;
		}
		tiles_.push_back(std::move(rowTiles));
		width_ = std::max(width_, col);
		++row;
	}
	height_ = row;
}
