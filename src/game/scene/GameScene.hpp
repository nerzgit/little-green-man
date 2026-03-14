#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../story/StoryContext.hpp"
#include "../story/StoryManager.hpp"
#include "Scene.hpp"
#include <memory>
#include <string>
#include <unordered_set>

class Camera;
class CollisionResolver;
class DialogueBox;
class FontAtlas;
class Map;
class Player;

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
	void drawYSorted(Renderer& renderer);

	std::unique_ptr<Player>            player_;
	std::unique_ptr<Map>               currentMap_;
	std::unique_ptr<CollisionResolver> collision_;
	std::unique_ptr<Camera>            camera_;
	std::unique_ptr<FontAtlas>         font_;
	std::unique_ptr<DialogueBox>       dialogueBox_;

	StoryManager                    storyManager_;
	std::unordered_set<std::string> storyFlags_;

	static constexpr const char* kFontName = "PixelMplus12-Regular";
	static constexpr float kFontSize      = 24.0f;
	static constexpr int   kFontAtlasSize = 2048;
};

#endif // GAME_SCENE_HPP
