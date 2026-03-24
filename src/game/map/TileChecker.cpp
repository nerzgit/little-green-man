#include "TileChecker.hpp"

#include "../GameConstants.hpp"
#include "MapCsvLoader.hpp"

bool TileChecker::isTileAt(const MapCsvLoader& loader, TileType type, float x, float y) {
	int col = static_cast<int>(x / GameConstants::kTileSize);
	int row = static_cast<int>(y / GameConstants::kTileSize);
	return TilePicker::getTile(loader, col, row).type == type;
}
