#ifndef TILESET_LOADER_HPP
#define TILESET_LOADER_HPP

#include "../../engine/graphics/Texture.hpp"
#include <string>

class Renderer;
class MapLoader;

// タイルセット画像を保持し、MapLoader のタイルデータを使って描画するクラス。
// CSV の読み込みは行わない。
class TilesetLoader {
public:
	TilesetLoader(const std::string& tilesetPath);

	void draw(Renderer& renderer, const MapLoader& mapLoader,
	          float displaySize) const;

private:
	static constexpr int kTilePixels = 32;

	Texture tileset_;
	int     tilesetCols_;
};

#endif // TILESET_LOADER_HPP
