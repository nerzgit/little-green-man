#ifndef TILE_TYPE_HPP
#define TILE_TYPE_HPP

enum class TileType { EMPTY, WALL, PLAYER_SPAWN, TRIGGER, ENEMY_SPAWN };

struct Tile {
	int      id;
	TileType type;
};

#endif // TILE_TYPE_HPP
