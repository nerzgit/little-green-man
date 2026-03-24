#ifndef TILE_PICKER_HPP
#define TILE_PICKER_HPP

#include "TileType.hpp"

class MapCsvLoader;

class TilePicker {
public:
	static Tile     getTile(const MapCsvLoader& loader, int col, int row);
	static TileType getTileType(const MapCsvLoader& loader, int col, int row);
};

#endif // TILE_PICKER_HPP
