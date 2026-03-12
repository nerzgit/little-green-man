#ifndef MAP_HPP
#define MAP_HPP

#include "../ColorOverlay.hpp"
#include "../ParallaxOverlay.hpp"
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
class LightSystem;

enum class MapEvent { None, PlayerDead };

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
	void             drawLight(Renderer& renderer);
	virtual void     end() {
	}

	int       getActivatedReceiverCount() const;
	void      setLightOverlayColor(int index, const glm::vec4& rgba);
	void      drawLightOverlay(Renderer& renderer, int windowWidth, int windowHeight) const;
	void      drawColorOverlay(Renderer& renderer, int windowWidth, int windowHeight) const;
	void      drawParallaxOverlay(Renderer& renderer, int windowWidth, int windowHeight, float playerX) const;
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
	void loadLightTileset(const std::string& path);
	void loadParallaxOverlay(const std::string& path, float speed = 0.3f);
	void loadLightSources(const std::string& path);
	void loadMirrors(const std::string& path);
	void loadReceivers(const std::string& path);

	ColorOverlay                                    colorOverlay_;
	ParallaxOverlay                                 parallaxOverlay_;
	std::unique_ptr<TilesetLoader>                  tileset_;
	std::vector<std::unique_ptr<Enemy>>             enemies_;
	std::vector<std::unique_ptr<CollisionResolver>> enemyResolvers_;
	std::unique_ptr<LightSystem>                    lightSystem_;

private:
	struct MapLayer {
		std::unique_ptr<MapLoader> loader;
		DrawLayer                  drawLayer;
	};

	std::vector<MapLayer> mapLayers_;
};

#endif // MAP_HPP
