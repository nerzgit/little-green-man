#ifndef TILE_CHECKER_HPP
#define TILE_CHECKER_HPP

#include "TilePicker.hpp"

class MapCsvLoader;

class TileChecker {
public:
	static bool isTileAt(const MapCsvLoader& loader, TileType type, float x, float y);
};

#endif // TILE_CHECKER_HPP
