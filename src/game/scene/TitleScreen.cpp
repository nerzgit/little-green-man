#include "TitleScreen.hpp"

#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp"  // GLFW/glfw3.h
#include "GameScene.hpp"
#include "SceneManager.hpp"

#include <cstdlib>  // exit

TitleScreen::TitleScreen(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	font_ = std::make_unique<FontAtlas>(
	  "assets/fonts/PixelMplus12-Regular.ttf", 24.0f, 2048);

	menuItems_ = { "スタート", "おわり" };
}

void TitleScreen::update(float deltaTime, SceneManager& sm) {
	const bool upNow    = InputManager::isKeyPressed(GLFW_KEY_UP);
	const bool downNow  = InputManager::isKeyPressed(GLFW_KEY_DOWN);
	const bool enterNow = InputManager::isKeyPressed(GLFW_KEY_ENTER);
	const bool spaceNow = InputManager::isKeyPressed(GLFW_KEY_SPACE);

	// 上下でカーソル移動（押した瞬間のみ）
	if (upNow && !prevUp_) {
		selectedIndex_ =
		  (selectedIndex_ - 1 + (int)menuItems_.size()) % (int)menuItems_.size();
	}
	if (downNow && !prevDown_) {
		selectedIndex_ = (selectedIndex_ + 1) % (int)menuItems_.size();
	}

	// Enter または Space で決定（押した瞬間のみ）
	if ((enterNow && !prevEnter_) || (spaceNow && !prevSpace_)) {
		onSelect(sm);
	}

	prevUp_    = upNow;
	prevDown_  = downNow;
	prevEnter_ = enterNow;
	prevSpace_ = spaceNow;
}

void TitleScreen::render(Renderer& renderer) {
	renderer.clear();

	const float cx = windowWidth_ / 2.0f;
	const float cy = windowHeight_ / 2.0f;

	// タイトル
	renderer.drawTextHUD(*font_, "Little Green Man",
	                     glm::vec2(cx - 190.0f, cy - 120.0f),
	                     2.0f, glm::vec3(0.2f, 0.8f, 0.2f));

	// メニュー
	const float menuStartY  = cy - 20.0f;
	const float menuSpacingY = 40.0f;

	for (int i = 0; i < (int)menuItems_.size(); ++i) {
		const bool     selected = (i == selectedIndex_);
		const glm::vec3 color   = selected ? glm::vec3(1.0f, 1.0f, 0.2f)
		                                   : glm::vec3(0.7f, 0.7f, 0.7f);

		const std::string prefix = selected ? "> " : "  ";
		const float        y      = menuStartY + i * menuSpacingY;

		renderer.drawTextHUD(*font_, prefix + menuItems_[i],
		                     glm::vec2(cx - 80.0f, y), 1.2f, color);
	}

	// 操作ヒント
	renderer.drawTextHUD(*font_, "↑↓ で選択  Enter で決定",
	                     glm::vec2(cx - 140.0f, windowHeight_ - 30.0f),
	                     0.8f, glm::vec3(0.4f, 0.4f, 0.4f));
}

void TitleScreen::onSelect(SceneManager& sm) {
	switch (selectedIndex_) {
	case 0:  // スタート
		sm.switchTo(
		  std::make_unique<GameScene>(sm.getWindowWidth(), sm.getWindowHeight()));
		break;
	case 1:  // おわり
		std::exit(0);
		break;
	}
}
