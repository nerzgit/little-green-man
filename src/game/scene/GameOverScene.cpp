#include "GameOverScene.hpp"

#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "SceneManager.hpp"
#include "TitleScreen.hpp"

#include <algorithm>

GameOverScene::GameOverScene(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	font_ = std::make_unique<FontAtlas>("assets/fonts/PixelMplus12-Regular.ttf",
	                                    24.0f, 2048);
}

void GameOverScene::update(float deltaTime, SceneManager& sm) {
	timer_ += deltaTime;

	const bool enterNow = InputManager::isKeyPressed(GLFW_KEY_ENTER);
	const bool spaceNow = InputManager::isKeyPressed(GLFW_KEY_SPACE);

	if ((enterNow && !prevEnter_) || (spaceNow && !prevSpace_)) {
		sm.switchTo(std::make_unique<TitleScreen>(sm.getWindowWidth(),
		                                          sm.getWindowHeight()));
	}

	prevEnter_ = enterNow;
	prevSpace_ = spaceNow;
}

void GameOverScene::render(Renderer& renderer) {
	renderer.clear();

	// フェードイン
	float alpha = std::min(timer_ / 1.0f, 1.0f);

	const float cx = windowWidth_ / 2.0f;
	const float cy = windowHeight_ / 2.0f;

	renderer.drawTextHUD(*font_, "GAME OVER",
	                     glm::vec2(cx - 110.0f, cy - 30.0f), 2.0f,
	                     glm::vec3(0.9f * alpha, 0.2f * alpha, 0.2f * alpha));

	if (alpha >= 1.0f) {
		renderer.drawTextHUD(*font_, "Enter でタイトルへ戻る",
		                     glm::vec2(cx - 130.0f, cy + 40.0f), 1.0f,
		                     glm::vec3(0.7f, 0.7f, 0.7f));
	}
}
