#include "SceneManager.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include <utility>

SceneManager::SceneManager(int windowWidth, int windowHeight, std::unique_ptr<Scene> initialScene)
    : windowWidth_(windowWidth), windowHeight_(windowHeight), current_(std::move(initialScene)) {
}

void SceneManager::switchTo(std::unique_ptr<Scene> scene) {
	next_ = std::move(scene);
}

void SceneManager::update(float deltaTime) {
	// フレーム冒頭で予約済みシーンへ切り替える（安全に前シーンを破棄）
	if (next_) {
		// ここで current_ を破棄してから next_ を current_ に移す
		// これにより、シーンの切り替えが安全に行われる（前シーンのリソースが解放される）
		current_ = std::move(next_);
	}
	if (current_) {
		current_->update(deltaTime, *this);
	}
}

void SceneManager::render(Renderer& renderer) {
	if (current_) {
		current_->render(renderer);
	}
}
