#include "GameManager.hpp"
#include <GLFW/glfw3.h>

GameManager::GameManager(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	// プレイヤーを画面中央に配置
	player_ = std::make_unique<Player>(
	  glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f));
}

void GameManager::update(float deltaTime) {
	updatePlayer(deltaTime);
}

void GameManager::render(Renderer& renderer) {
	renderer.clear();

	// プレイヤーを描画
	if (player_->active) {
		renderer.drawCircle(player_->position, player_->size, 0.2f, 0.8f, 0.2f);
	}
}

void GameManager::updatePlayer(float deltaTime) {
	player_->update(deltaTime);

	// 画面外に出ないようにする
	if (player_->position.x < player_->size) {
		player_->position.x = player_->size;
	}
	if (player_->position.x > windowWidth_ - player_->size) {
		player_->position.x = windowWidth_ - player_->size;
	}
	if (player_->position.y < player_->size) {
		player_->position.y = player_->size;
	}
	if (player_->position.y > windowHeight_ - player_->size) {
		player_->position.y = windowHeight_ - player_->size;
	}
}