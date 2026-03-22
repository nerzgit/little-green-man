#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP

#include "../../engine/font/FontAtlas.hpp"
#include "Scene.hpp"
#include <memory>

// 起動時のスプラッシュ画面。一定時間後に TitleScreen へ自動遷移する。
// Space キーでスキップ可能。
class SplashScreen : public Scene {
public:
	SplashScreen(int windowWidth, int windowHeight);

	void update(float deltaTime, SceneManager& sm) override;
	void render(Renderer& renderer) override;

private:
	int   windowWidth_;
	int   windowHeight_;
	float timer_ = 0.0f;

	static constexpr float DURATION = 2.5f; // 表示秒数
	static constexpr float FADE_IN  = 0.5f; // フェードイン時間
	static constexpr float FADE_OUT = 0.5f; // フェードアウト時間

	std::unique_ptr<FontAtlas> font_;

	void goNext(SceneManager& sm);
};

#endif // SPLASH_SCREEN_HPP
