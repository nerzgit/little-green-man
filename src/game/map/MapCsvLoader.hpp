#ifndef MAP_CSV_LOADER_HPP
#define MAP_CSV_LOADER_HPP

#include "TilePicker.hpp"
#include <string>
#include <vector>

class TilePicker;
class TileChecker;

class MapCsvLoader {
	friend class TilePicker;
	friend class TileChecker;
public:
	MapCsvLoader()  = default;
	~MapCsvLoader() = default;

	void loadMap(const std::string& path, TileType tileType = TileType::EMPTY);

	int  getWidth()  const { return width_; }
	int  getHeight() const { return height_; }

	int  getPlayerSpawnCol() const { return playerSpawnCol_; }
	int  getPlayerSpawnRow() const { return playerSpawnRow_; }
	bool hasPlayerSpawn()    const { return hasPlayerSpawn_; }
	const std::vector<std::pair<int, int>>& getEnemySpawns() const { return enemySpawns_; }

private:
	std::vector<std::vector<Tile>> tiles_;
	int  width_         = 0;
	int  height_        = 0;
	int  playerSpawnCol_ = 0;
	int  playerSpawnRow_ = 0;
	bool hasPlayerSpawn_ = false;
	std::vector<std::pair<int, int>> enemySpawns_; // {col, row}
};

#endif // MAP_CSV_LOADER_HPP
