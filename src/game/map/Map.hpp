#ifndef MAP_HPP
#define MAP_HPP

#include <memory>

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

	bool  isWallAt(float x, float y) const;
	float getPixelWidth() const;
	float getPixelHeight() const;

protected:
	std::unique_ptr<MapLoader> mapLoader_;
};

#endif // MAP_HPP
