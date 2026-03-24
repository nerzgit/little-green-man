#include "TilePicker.hpp"

#include "MapCsvLoader.hpp"

Tile TilePicker::getTile(const MapCsvLoader& loader, int col, int row) {
	if (row < 0 || row >= loader.height_ || col < 0
	    || col >= static_cast<int>(loader.tiles_[row].size())) {
		return {0, TileType::EMPTY};
	}
	return loader.tiles_[row][col];
}

TileType TilePicker::getTileType(const MapCsvLoader& loader, int col, int row) {
	return getTile(loader, col, row).type;
}
