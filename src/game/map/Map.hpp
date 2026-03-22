#ifndef MAP_HPP
#define MAP_HPP

#include "../../engine/core/Entity.hpp"
#include "../effect/ColorOverlay.hpp"
#include "../IDrawableSource.hpp"
#include "../effect/ParallaxOverlay.hpp"
#include "TilesetLoader.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../Drawable.hpp"
#include "MapLoader.hpp"

class Renderer;
class Player;
class World;
class Texture;
class LightSystem;
class EnemySystem;

enum class MapEvent { None, PlayerDead };

enum class DrawLayer { Background, Foreground };

class Map {
public:
	Map();
	virtual ~Map();

	virtual void     start();
	virtual MapEvent update(float deltaTime, Player& player);
	void             draw(Renderer& renderer, DrawLayer layer);
	void             collectDrawables(std::vector<Drawable>& out);
	virtual void     end() {
	}

	int  getActivatedReceiverCount() const;
	void drawEffects(Renderer& renderer, World& world) const;
	bool      isWallAt(float x, float y) const;
	bool      isTriggerAt(float x, float y) const;
	bool      isMirrorAt(float x, float y) const;
	void      resolvePush(Entity& pusher);
	float     getPixelWidth() const;
	float     getPixelHeight() const;
	glm::vec2 getPlayerSpawnPosition() const;

	// 次のマップを生成して返す。なければ nullptr。
	virtual std::unique_ptr<Map> createNextMap() {
		return nullptr;
	}

	// このマップに紐づくストーリー JSON のパスを返す。なければ空文字。
	virtual std::string getStoryPath() const {
		return "";
	}

protected:
	void loadMap(const std::string& csvPath,
	             TileType           defaultType = TileType::WALL,
	             DrawLayer          drawLayer   = DrawLayer::Background);
	void loadTileset(const std::string& tilesetPath);

	ColorOverlay                   colorOverlay_;
	ParallaxOverlay                parallaxOverlay_;
	std::unique_ptr<TilesetLoader> tileset_;
	std::unique_ptr<LightSystem>   lightSystem_;
	std::unique_ptr<EnemySystem>   enemySystem_;

	std::vector<IDrawableSource*> drawableSources_;

private:
	struct MapLayer {
		std::unique_ptr<MapLoader> loader;
		DrawLayer                  drawLayer;
	};

	std::vector<MapLayer> mapLayers_;
};

#endif // MAP_HPP
