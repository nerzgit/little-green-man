#include "../../engine/graphics/AnimatedSprite.hpp"
#include "../../engine/graphics/Renderer.hpp"
#include "../../engine/graphics/SpriteSheet.hpp"
#include "../../engine/graphics/TextureCache.hpp"

#include "Player.hpp"

#include "../../engine/input/InputManager.hpp"
#include <GLFW/glfw3.h>

Player::Player() : Entity(glm::vec2(0, 0), 64.0f), speed(200.0f) {
	sprite_ = std::make_unique<AnimatedSprite>();

	// TODO:
	// スプライトシートのパスとフレームサイズを実ファイルに合わせて変更する
	SpriteSheet sheet(
	  TextureCache::get("assets/characters/player/main-character.png"), 64, 88);

	Animation idle;
	idle.frameDuration = 1.0f;
	idle.frames.push_back(sheet.frame(0, 0));
	idle.frames.push_back(sheet.frame(1, 0));
	idle.frames.push_back(sheet.frame(2, 0));
	sprite_->addAnimation("idle", std::move(idle));

	Animation walkRight;
	walkRight.frameDuration = 0.1f;
	walkRight.frames.push_back(sheet.frame(0, 1));
	walkRight.frames.push_back(sheet.frame(1, 1));
	walkRight.frames.push_back(sheet.frame(2, 1));
	sprite_->addAnimation("walk_right", std::move(walkRight));

	Animation walkLeft;
	walkLeft.frameDuration = 0.1f;
	walkLeft.frames.push_back(sheet.frame(0, 2));
	walkLeft.frames.push_back(sheet.frame(1, 2));
	walkLeft.frames.push_back(sheet.frame(2, 2));
	sprite_->addAnimation("walk_left", std::move(walkLeft));

	Animation walkUp;
	walkUp.frameDuration = 0.1f;
	walkUp.frames.push_back(sheet.frame(0, 4));
	walkUp.frames.push_back(sheet.frame(1, 4));
	walkUp.frames.push_back(sheet.frame(2, 4));
	sprite_->addAnimation("walk_up", std::move(walkUp));

	Animation walkDown;
	walkDown.frameDuration = 0.2f;
	walkDown.frames.push_back(sheet.frame(0, 3));
	walkDown.frames.push_back(sheet.frame(1, 3));
	walkDown.frames.push_back(sheet.frame(2, 3));
	sprite_->addAnimation("walk_down", std::move(walkDown));

	sprite_->play("idle");
}

Player::~Player() = default;

void Player::update(float deltaTime) {
	handleInput(deltaTime);
	Entity::update(deltaTime);
	sprite_->update(deltaTime);
}

void Player::draw(Renderer& renderer) {
	const auto& frame = sprite_->currentFrame();
	renderer.drawSprite(*frame.texture, {position.x, position.y + size / 2.0f},
	                    64.0f, 88.0f, SpriteAnchor::BOTTOM, frame.uvMin,
	                    frame.uvMax);
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
