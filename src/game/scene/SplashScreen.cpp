#include "SplashScreen.hpp"

#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "../GameConstants.hpp"
#include "SceneManager.hpp"
#include "TitleScreen.hpp"

#include <algorithm>

SplashScreen::SplashScreen(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	font_ = std::make_unique<FontAtlas>("assets/fonts/PixelMplus12-Regular.ttf",
	                                    24.0f, 2048);
}

void SplashScreen::update(float deltaTime, SceneManager& sm) {
	timer_ += deltaTime;

	if (InputManager::isKeyPressed(GLFW_KEY_SPACE) || timer_ >= DURATION) {
		goNext(sm);
	}
}

void SplashScreen::render(Renderer& renderer) {
	renderer.clear();

	// フェードイン / フェードアウトのアルファ計算
	float alpha = 1.0f;
	if (timer_ < FADE_IN) {
		alpha = timer_ / FADE_IN;
	} else if (timer_ > DURATION - FADE_OUT) {
		alpha = (DURATION - timer_) / FADE_OUT;
	}
	alpha = std::clamp(alpha, 0.0f, 1.0f);

	const glm::vec3 green(0.2f * alpha, 0.8f * alpha, 0.2f * alpha);
	const glm::vec3 white(alpha, alpha, alpha);

	const float cx = windowWidth_ / 2.0f;
	const float cy = windowHeight_ / 2.0f;

	// タイトル（大きめに scale 2x）
	renderer.drawTextHUD(*font_, GameConstants::kGameTitle,
	                     glm::vec2(cx - 190.0f, cy - 20.0f), 2.0f, green);

	// サブコピー
	renderer.drawTextHUD(*font_, "nerzgit 2026",
	                     glm::vec2(cx - 70.0f, cy + 30.0f), 1.0f, white);

	// スキップヒント
	renderer.drawTextHUD(*font_, "SPACE でスキップ",
	                     glm::vec2(cx - 90.0f, windowHeight_ - 30.0f), 0.8f,
	                     glm::vec3(0.5f * alpha, 0.5f * alpha, 0.5f * alpha));
}

void SplashScreen::goNext(SceneManager& sm) {
	sm.switchTo(
	  std::make_unique<TitleScreen>(sm.getWindowWidth(), sm.getWindowHeight()));
}
