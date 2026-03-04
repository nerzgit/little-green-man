#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "../../engine/core/Entity.hpp"

class Renderer;

// 左右に往復パトロールする敵。
// patrolDistance: 初期位置から左右それぞれ何ピクセル移動するか
class Enemy : public Entity {
public:
	Enemy(glm::vec2 position, float patrolDistance, float speed = 120.0f);

	void update(float deltaTime) override;
	void draw(Renderer& renderer);

private:
	float originX_;
	float patrolDistance_;
	float speed_;
};

#endif // ENEMY_HPP
