#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "Scene.hpp"
#include "../story/StoryContext.hpp"
#include "../story/StoryManager.hpp"
#include <memory>
#include <string>
#include <unordered_set>

class Camera;
class CollisionResolver;
class DialogueBox;
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
	std::unique_ptr<DialogueBox>       dialogueBox_;

	StoryManager                       storyManager_;
	std::unordered_set<std::string>    storyFlags_;
};

#endif // GAME_SCENE_HPP
