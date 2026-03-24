#include "MapLayer.hpp"

MapLayer::MapLayer(const std::string& csvPath, TileType defaultType, DrawLayer drawLayer)
    : loader_(std::make_unique<MapCsvLoader>()), drawLayer_(drawLayer) {
	loader_->loadMap(csvPath, defaultType);
}
