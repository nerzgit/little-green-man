#include "World.hpp"

#include "camera/Camera.hpp"
#include "enemy/EnemySystem.hpp"
#include "map/Map.hpp"
#include "physics/CollisionResolver.hpp"
#include "player/Player.hpp"
#include <glm/glm.hpp>

World::~World() = default;

World::World(int windowWidth, int windowHeight)
    : enemySystem_(std::make_unique<EnemySystem>()),
      windowWidth_(windowWidth), windowHeight_(windowHeight) {
	player_ = std::make_unique<Player>();
}

void World::loadMap(std::unique_ptr<Map> map) {
	currentMap_ = std::move(map);

	enemySystem_ = std::make_unique<EnemySystem>();
	enemySystem_->spawnFrom(currentMap_->getSpawnPositions(CharacterType::Enemy), *currentMap_);

	player_->position = currentMap_->getSpawnPosition(CharacterType::Player);
	player_->velocity = glm::vec2(0, 0);

	collision_ = std::make_unique<CollisionResolver>(*player_, *currentMap_);
	const auto mapSize = currentMap_->getMapSize();
	camera_    = std::make_unique<Camera>(*player_, windowWidth_, windowHeight_,
	                                      mapSize.width, mapSize.height);
}

bool World::update(float deltaTime) {
	return enemySystem_->update(deltaTime, *player_);
}

Player&            World::player()      { return *player_; }
Map&               World::map()         { return *currentMap_; }
Camera&            World::camera()      { return *camera_; }
CollisionResolver& World::collision()   { return *collision_; }
EnemySystem&       World::enemySystem() { return *enemySystem_; }
int                World::windowWidth()  const { return windowWidth_; }
int                World::windowHeight() const { return windowHeight_; }
