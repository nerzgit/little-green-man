#ifndef MAP_HPP
#define MAP_HPP

#include "../enemy/Enemy.hpp"
#include "../physics/CollisionResolver.hpp"
#include "wall/GrayBlock.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class MapLoader;
class Renderer;
class Player;

enum class MapEvent { None, NextMap, PlayerDead };

class Map {
public:
	Map();
	virtual ~Map();

	virtual void     start();
	virtual MapEvent update(float deltaTime, Player& player);
	virtual void     draw(Renderer& renderer);
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
	std::unique_ptr<MapLoader>          mapLoader_;
	GrayBlock                           wallBlock_;
	std::vector<std::unique_ptr<Enemy>>             enemies_;
	std::vector<std::unique_ptr<CollisionResolver>> enemyResolvers_;
};

#endif // MAP_HPP
