#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <memory>

class Camera;
class CollisionResolver;
class Map;
class Player;
class Renderer;

class GameManager {
public:
	GameManager(int windowWidth, int windowHeight);
	~GameManager();

	void update(float deltaTime);
	void render(Renderer& renderer);

private:
	static constexpr float kStageWidth  = 1600.0f;
	static constexpr float kStageHeight = 1200.0f;

	int windowWidth_;
	int windowHeight_;

	std::unique_ptr<Player>            player_;
	std::unique_ptr<Map>               currentMap_;
	std::unique_ptr<CollisionResolver> collision_;
	std::unique_ptr<Camera>            camera_;
};

#endif // GAME_MANAGER_HPP
