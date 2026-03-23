#ifndef MAP_HPP
#define MAP_HPP

#include "MapLoader.hpp"
#include "TilesetLoader.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class Renderer;
class Player;
class EnemySystem;

enum class DrawLayer { Background, Foreground };
enum class MapEvent  { None, PlayerDead };

class Map {
public:
	// JSON ファイルからマップを生成するファクトリ
	static std::unique_ptr<Map> load(const std::string& jsonPath);

	Map();
	~Map();

	void     start();
	MapEvent update(float deltaTime, Player& player);
	void     draw(Renderer& renderer, DrawLayer layer);

	bool      isWallAt(float x, float y) const;
	bool      isTriggerAt(float x, float y) const;
	float     getPixelWidth() const;
	float     getPixelHeight() const;
	glm::vec2 getPlayerSpawnPosition() const;

	const EnemySystem& enemySystem() const;

	// 次マップが存在しない場合は nullptr
	std::unique_ptr<Map> createNextMap() const;
	const std::string&   getStoryPath() const;

private:
	void loadLayer(const std::string& csvPath, TileType defaultType,
	               DrawLayer drawLayer);
	void loadTileset(const std::string& tilesetPath);

	std::vector<glm::vec2> getEnemySpawnPositions() const;

	struct Layer {
		std::unique_ptr<MapLoader> loader;
		DrawLayer                  drawLayer;
	};
	std::vector<Layer>             layers_;
	std::unique_ptr<TilesetLoader> tileset_;
	std::unique_ptr<EnemySystem>   enemySystem_;

	std::string nextMapPath_;
	std::string storyPath_;
};

#endif // MAP_HPP
