#ifndef MAP_HPP
#define MAP_HPP

#include "../enemy/Enemy.hpp"
#include "../physics/CollisionResolver.hpp"
#include "TilesetLoader.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "MapLoader.hpp"

class Renderer;
class Player;
class Texture;

enum class MapEvent { None, NextMap, PlayerDead };

enum class DrawLayer { Background, Foreground };

class Map {
public:
	Map();
	virtual ~Map();

	virtual void     start();
	virtual MapEvent update(float deltaTime, Player& player);
	void             drawBackground(Renderer& renderer);
	void             drawForeground(Renderer& renderer);
	void             drawEnemy(Renderer& renderer);
	virtual void     end() {
	}

	bool      isWallAt(float x, float y) const;
	bool      isTriggerAt(float x, float y) const;
	float     getPixelWidth() const;
	float     getPixelHeight() const;
	glm::vec2 getPlayerSpawnPosition() const;

	// 次のマップを生成して返す。なければ nullptr。
	virtual std::unique_ptr<Map> createNextMap() {
		return nullptr;
	}

protected:
	void loadMap(const std::string& csvPath,
	             TileType           defaultType = TileType::WALL,
	             DrawLayer          drawLayer   = DrawLayer::Background);
	void loadTileset(const std::string& tilesetPath);

	std::unique_ptr<TilesetLoader>                  tileset_;
	std::vector<std::unique_ptr<Enemy>>             enemies_;
	std::vector<std::unique_ptr<CollisionResolver>> enemyResolvers_;

private:
	struct MapLayer {
		std::unique_ptr<MapLoader> loader;
		DrawLayer                  drawLayer;
	};

	std::vector<MapLayer> mapLayers_;
};

#endif // MAP_HPP
