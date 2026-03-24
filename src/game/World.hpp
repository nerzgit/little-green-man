#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

class Camera;
class CollisionResolver;
class EnemySystem;
class Map;
class Player;

class World {
public:
	World(int windowWidth, int windowHeight);
	~World();

	void loadMap(std::unique_ptr<Map> map);

	// 敵 AI を更新する。プレイヤーが死亡した場合 true を返す
	bool update(float deltaTime);

	Player&            player();
	Map&               map();
	Camera&            camera();
	CollisionResolver& collision();
	EnemySystem&       enemySystem();

	int windowWidth()  const;
	int windowHeight() const;

private:
	std::unique_ptr<Player>            player_;
	std::unique_ptr<Map>               currentMap_;
	std::unique_ptr<Camera>            camera_;
	std::unique_ptr<CollisionResolver> collision_;
	std::unique_ptr<EnemySystem>       enemySystem_;
	int                                windowWidth_;
	int                                windowHeight_;
};

#endif // WORLD_HPP
