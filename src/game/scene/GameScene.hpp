#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../World.hpp"
#include "../story/StoryManager.hpp"
#include "Scene.hpp"
#include <string>
#include <unordered_set>

class GameScene : public Scene {
public:
	GameScene(int windowWidth, int windowHeight);

	void update(float deltaTime, SceneManager& sm) override;
	void render(Renderer& renderer) override;

private:
	void loadMap(std::unique_ptr<class Map> newMap);

	World                           world_;
	StoryManager                    storyManager_;
	std::unordered_set<std::string> storyFlags_;
};

#endif // GAME_SCENE_HPP
