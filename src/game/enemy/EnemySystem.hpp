#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include "../IDrawableSource.hpp"
#include "Enemy.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Map;
class CollisionResolver;

class EnemySystem : public IDrawableSource {
public:
	EnemySystem();
	~EnemySystem();

	void spawnFrom(const std::vector<glm::vec2>& positions, float stageWidth,
	               float stageHeight, Map& map);

	// 敵の更新・衝突解決。プレイヤーに当たった場合 true を返す
	bool update(float deltaTime, class Player& player);

	void collectDrawables(std::vector<Drawable>& out) const override;

private:
	std::vector<std::unique_ptr<Enemy>>             enemies_;
	std::vector<std::unique_ptr<CollisionResolver>> resolvers_;
};

#endif // ENEMY_SYSTEM_HPP
