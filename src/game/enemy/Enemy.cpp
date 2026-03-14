#include "Enemy.hpp"

#include "../../engine/graphics/Renderer.hpp"

Enemy::Enemy(glm::vec2 position, float patrolDistance, float speed)
    : Entity(position, 48.0f)
    , originX_(position.x)
    , patrolDistance_(patrolDistance)
    , speed_(speed) {
	velocity.x = speed_;
}

void Enemy::update(float deltaTime) {
	Entity::update(deltaTime);

	// 折り返し判定
	if (position.x > originX_ + patrolDistance_) {
		position.x = originX_ + patrolDistance_;
		velocity.x = -speed_;
	} else if (position.x < originX_ - patrolDistance_) {
		position.x = originX_ - patrolDistance_;
		velocity.x = speed_;
	}
}

void Enemy::draw(Renderer& renderer) {
	constexpr float kSize = 48.0f;
	renderer.drawRect(position, kSize, kSize, glm::vec3(0.9f, 0.2f, 0.2f));
}
