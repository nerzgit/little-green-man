#include "GameManager.hpp"
#include "PlayerSprite.hpp"
#include <GLFW/glfw3.h>

GameManager::GameManager(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	player_ = std::make_unique<Player>(
	  glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f));

	playerTexture_ = createPlayerTexture();
}

void GameManager::update(float deltaTime) {
	updatePlayer(deltaTime);
}

void GameManager::render(Renderer& renderer) {
	renderer.clear();

	if (player_->active) {
		constexpr float kSpriteSize = 64.0f;
		renderer.drawSprite(*playerTexture_, player_->position, kSpriteSize,
		                    kSpriteSize);
	}
}

void GameManager::updatePlayer(float deltaTime) {
	player_->update(deltaTime);

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
