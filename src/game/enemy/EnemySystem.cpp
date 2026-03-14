#include "EnemySystem.hpp"

#include "../physics/CollisionResolver.hpp"
#include "../player/Player.hpp"

EnemySystem::EnemySystem()  = default;
EnemySystem::~EnemySystem() = default;

void EnemySystem::spawnFrom(const std::vector<glm::vec2>& positions,
                             float stageWidth, float stageHeight, Map& map) {
	enemies_.clear();
	resolvers_.clear();

	for (const auto& pos : positions) {
		enemies_.emplace_back(std::make_unique<Enemy>(pos, 150.0f));
		resolvers_.emplace_back(std::make_unique<CollisionResolver>(
		  *enemies_.back(), map, stageWidth, stageHeight));
	}
}

bool EnemySystem::update(float deltaTime, Player& player) {
	for (size_t i = 0; i < enemies_.size(); ++i) {
		enemies_[i]->update(deltaTime);
		resolvers_[i]->resolve();
		if (player.intersects(*enemies_[i]))
			return true;
	}
	return false;
}

void EnemySystem::collectDrawables(std::vector<Drawable>& out) const {
	for (const auto& enemy : enemies_) {
		out.push_back({enemy->position.y, [e = enemy.get()](Renderer& r) {
			                e->draw(r);
		                }});
	}
}
