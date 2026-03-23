#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include "Enemy.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Map;
class CollisionResolver;

class EnemySystem {
public:
	EnemySystem();
	~EnemySystem();

	void spawnFrom(const std::vector<glm::vec2>& positions, Map& map);

	// 敵の更新・衝突解決。プレイヤーに当たった場合 true を返す
	bool update(float deltaTime, class Player& player);

	const std::vector<std::unique_ptr<Enemy>>& enemies() const;

private:
	std::vector<std::unique_ptr<Enemy>>             enemies_;
	std::vector<std::unique_ptr<CollisionResolver>> resolvers_;
};

#endif // ENEMY_SYSTEM_HPP
