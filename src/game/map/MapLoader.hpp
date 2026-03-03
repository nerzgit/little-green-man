#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

#include <string>
#include <vector>

enum class TileType { EMPTY, WALL, PLAYER_SPAWN, TRIGGER };

class MapLoader {
public:
	static constexpr float kTileSize = 64.0f;

	MapLoader();
	~MapLoader();

	void load(const std::string& path);

	TileType getTile(int col, int row) const;
	bool     isWallAt(float x, float y) const;
	bool     isTriggerAt(float x, float y) const;
	int      getWidth() const {
        return width_;
	}
	int getHeight() const {
		return height_;
	}
	int getPlayerSpawnCol() const {
		return playerSpawnCol_;
	}
	int getPlayerSpawnRow() const {
		return playerSpawnRow_;
	}

private:
	std::vector<std::vector<TileType>> tiles_;
	int                                width_          = 0;
	int                                height_         = 0;
	int                                playerSpawnCol_ = 0;
	int                                playerSpawnRow_ = 0;
};

#endif // MAP_LOADER_HPP
