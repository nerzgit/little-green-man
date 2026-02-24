#include "Player.hpp"
#include "../engine/input/InputManager.hpp"
#include <GLFW/glfw3.h>

Player::Player(glm::vec2 position) : Entity(position, 15.0f), speed(200.0f) {
}

void Player::update(float deltaTime) {
	handleInput(deltaTime);
	Entity::update(deltaTime);
}

void Player::handleInput(float deltaTime) {
	glm::vec2 direction(0, 0);

	if (InputManager::isKeyPressed(GLFW_KEY_W)) {
		direction.y -= 1.0f;
	}
	if (InputManager::isKeyPressed(GLFW_KEY_S)) {
		direction.y += 1.0f;
	}
	if (InputManager::isKeyPressed(GLFW_KEY_A)) {
		direction.x -= 1.0f;
	}
	if (InputManager::isKeyPressed(GLFW_KEY_D)) {
		direction.x += 1.0f;
	}

	if (glm::length(direction) > 0) {
		velocity = glm::normalize(direction) * speed;
	} else {
		velocity = glm::vec2(0, 0);
	}
}
