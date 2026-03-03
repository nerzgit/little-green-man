#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <memory>

class Camera;
class CollisionResolver;
class FontAtlas;
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
	int windowWidth_;
	int windowHeight_;

	std::unique_ptr<Player>            player_;
	std::unique_ptr<Map>               currentMap_;
	std::unique_ptr<CollisionResolver> collision_;
	std::unique_ptr<Camera>            camera_;
	std::unique_ptr<FontAtlas>         font_;
};

#endif // GAME_MANAGER_HPP
