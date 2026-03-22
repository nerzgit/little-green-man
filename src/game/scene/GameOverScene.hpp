#ifndef GAME_OVER_SCENE_HPP
#define GAME_OVER_SCENE_HPP

#include "../../engine/font/FontAtlas.hpp"
#include "Scene.hpp"
#include <memory>

// ゲームオーバー画面。Enter でタイトルへ戻る。
class GameOverScene : public Scene {
public:
	GameOverScene(int windowWidth, int windowHeight);

	void update(float deltaTime, SceneManager& sm) override;
	void render(Renderer& renderer) override;

private:
	int windowWidth_;
	int windowHeight_;

	float timer_ = 0.0f;

	std::unique_ptr<FontAtlas> font_;

	bool prevEnter_ = false;
	bool prevSpace_ = false;
};

#endif // GAME_OVER_SCENE_HPP
