#ifndef MAP_LAYER_HPP
#define MAP_LAYER_HPP

#include "MapCsvLoader.hpp"
#include <memory>
#include <string>

enum class DrawLayer { Background, Foreground };

class MapLayer {
public:
	MapLayer(const std::string& csvPath, TileType defaultType, DrawLayer drawLayer);

	const MapCsvLoader& loader()    const { return *loader_; }
	DrawLayer           drawLayer() const { return drawLayer_; }

private:
	std::unique_ptr<MapCsvLoader> loader_;
	DrawLayer                     drawLayer_;
};

#endif // MAP_LAYER_HPP
