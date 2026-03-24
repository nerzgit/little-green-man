#ifndef TILESET_HPP
#define TILESET_HPP

#include "../../engine/graphics/Texture.hpp"
#include <memory>
#include <string>

class Renderer;
class MapCsvLoader;

// タイルセット画像を保持し、MapCsvLoader のタイルデータを使って描画するクラス。
// CSV の読み込みは行わない。
class Tileset {
public:
	Tileset(const std::string& tilesetPath);

	void draw(Renderer& renderer, const MapCsvLoader& mapLoader,
	          float displaySize) const;

private:
	std::shared_ptr<Texture> tileset_;
	int                      tilesetCols_;
};

#endif // TILESET_HPP
