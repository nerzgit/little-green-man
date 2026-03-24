#ifndef MAP_HPP
#define MAP_HPP

#include "../CharacterType.hpp"
#include "MapLayer.hpp"
#include "Tileset.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class Renderer;

struct MapSize {
	float width;
	float height;
};

class Map {
public:
	// JSON ファイルからマップを生成するファクトリ
	static std::unique_ptr<Map> load(const std::string& jsonPath);

	Map();
	~Map();

	void draw(Renderer& renderer, DrawLayer layer);

	bool      isTileAt(TileType type, float x, float y) const;
	MapSize   getMapSize() const;
	glm::vec2              getSpawnPosition(CharacterType type) const;
	std::vector<glm::vec2> getSpawnPositions(CharacterType type) const;

	// 次マップが存在しない場合は nullptr
	std::unique_ptr<Map> createNextMap() const;
	const std::string&   getStoryPath() const;

private:
	void loadTileset(const std::string& tilesetPath);

	std::vector<MapLayer>    layers_;
	std::unique_ptr<Tileset> tileset_;

	std::string nextMapPath_;
	std::string storyPath_;
};

#endif // MAP_HPP
