#include "GameManager.hpp"

#include "../engine/graphics/Renderer.hpp"
#include "Player.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>

GameManager::GameManager(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	player_ = std::make_unique<Player>(
	  glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f));
}

GameManager::~GameManager() = default;

void GameManager::update(float deltaTime) {
	updatePlayer(deltaTime);
}

void GameManager::render(Renderer& renderer) {
	updateCamera(renderer);
	renderer.clear();

	if (player_->active) {
		constexpr float kSpriteSize = 64.0f;
		renderer.drawSprite(player_->getSprite(), player_->position,
		                    kSpriteSize, kSpriteSize);
	}
}

void GameManager::updatePlayer(float deltaTime) {
	player_->update(deltaTime);

	player_->position.x = std::clamp(player_->position.x, player_->size,
	                                 kStageWidth - player_->size);
	player_->position.y = std::clamp(player_->position.y, player_->size,
	                                 kStageHeight - player_->size);
}

void GameManager::updateCamera(Renderer& renderer) {
	constexpr float zoom  = 2.0f;
	float           viewW = static_cast<float>(windowWidth_) * zoom;
	float           viewH = static_cast<float>(windowHeight_) * zoom;

	cameraPos_.x = player_->position.x - viewW / 2.0f;
	cameraPos_.y = player_->position.y - viewH / 2.0f;
	cameraPos_.x = std::clamp(cameraPos_.x, 0.0f, kStageWidth - viewW);
	cameraPos_.y = std::clamp(cameraPos_.y, 0.0f, kStageHeight - viewH);

	renderer.setCamera(cameraPos_);
}
