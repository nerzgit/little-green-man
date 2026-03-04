#ifndef MAP_HPP
#define MAP_HPP

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class MapLoader;
class Renderer;

class Map {
public:
	Map();
	virtual ~Map();

	virtual void onStart() {
	}
	virtual void onUpdate(float deltaTime) {
	}
	virtual void onDraw(Renderer& renderer) {
	}
	virtual void onEnd() {
	}

	bool      isWallAt(float x, float y) const;
	bool      isTriggerAt(float x, float y) const;
	float     getPixelWidth() const;
	float     getPixelHeight() const;
	glm::vec2 getPlayerSpawnPosition() const;
	std::vector<glm::vec2> getEnemySpawnPositions() const;

	// 次のマップを生成して返す。なければ nullptr。
	virtual std::unique_ptr<Map> createNextMap() { return nullptr; }

protected:
	std::unique_ptr<MapLoader> mapLoader_;
};

#endif // MAP_HPP
