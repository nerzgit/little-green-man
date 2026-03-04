#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "Scene.hpp"
#include <memory>
#include <vector>

class Camera;
class CollisionResolver;
class Enemy;
class FontAtlas;
class Map;
class Player;

// ゲーム本体のシーン。
class GameScene : public Scene {
public:
	GameScene(int windowWidth, int windowHeight);
	~GameScene() override;

	void update(float deltaTime, SceneManager& sm) override;
	void render(Renderer& renderer) override;

private:
	int windowWidth_;
	int windowHeight_;

	void loadMap(std::unique_ptr<Map> newMap);

	std::unique_ptr<Player>            player_;
	std::unique_ptr<Map>               currentMap_;
	std::unique_ptr<CollisionResolver> collision_;
	std::unique_ptr<Camera>            camera_;
	std::unique_ptr<FontAtlas>         font_;

	std::vector<std::unique_ptr<Enemy>> enemies_;
};

#endif // GAME_SCENE_HPP
