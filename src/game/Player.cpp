#include "../engine/graphics/AnimatedSprite.hpp"

#include "Player.hpp"

#include "../engine/graphics/Texture.hpp"
#include "../engine/input/InputManager.hpp"
#include <GLFW/glfw3.h>

Player::Player(glm::vec2 position) : Entity(position, 32.0f), speed(400.0f) {
	sprite_ = std::make_unique<AnimatedSprite>();

	Animation idle;
	idle.frameDuration = 1.0f;
	idle.frames.push_back(std::make_shared<Texture>("assets/char_1.png"));
	idle.frames.push_back(std::make_shared<Texture>("assets/char_2.png"));
	idle.frames.push_back(std::make_shared<Texture>("assets/char_3.png"));
	sprite_->addAnimation("idle", std::move(idle));

	Animation walkRight;
	walkRight.frameDuration = 0.1f;
	walkRight.frames.push_back(std::make_shared<Texture>("assets/char_1.png"));
	walkRight.frames.push_back(std::make_shared<Texture>("assets/char_3.png"));
	sprite_->addAnimation("walk_right", std::move(walkRight));

	Animation walkLeft;
	walkLeft.frameDuration = 0.1f;
	walkLeft.frames.push_back(std::make_shared<Texture>("assets/char_1.png"));
	walkLeft.frames.push_back(std::make_shared<Texture>("assets/char_2.png"));
	sprite_->addAnimation("walk_left", std::move(walkLeft));

	Animation walkUp;
	walkUp.frameDuration = 0.1f;
	walkUp.frames.push_back(std::make_shared<Texture>("assets/char_2.png"));
	walkUp.frames.push_back(std::make_shared<Texture>("assets/char_3.png"));
	sprite_->addAnimation("walk_up", std::move(walkUp));

	Animation walkDown;
	walkDown.frameDuration = 0.1f;
	walkDown.frames.push_back(std::make_shared<Texture>("assets/char_3.png"));
	walkDown.frames.push_back(std::make_shared<Texture>("assets/char_1.png"));
	sprite_->addAnimation("walk_down", std::move(walkDown));

	sprite_->play("idle");
}

Player::~Player() = default;

void Player::update(float deltaTime) {
	handleInput(deltaTime);
	Entity::update(deltaTime);
	sprite_->update(deltaTime);
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

	if (velocity.x > 0) {
		sprite_->play("walk_right");
	} else if (velocity.x < 0) {
		sprite_->play("walk_left");
	} else if (velocity.y < 0) {
		sprite_->play("walk_up");
	} else if (velocity.y > 0) {
		sprite_->play("walk_down");
	} else {
		sprite_->play("idle");
	}
}

const Texture& Player::getSprite() const {
	return sprite_->currentTexture();
}