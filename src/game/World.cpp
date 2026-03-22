#include "World.hpp"

#include "camera/Camera.hpp"
#include "map/Map.hpp"
#include "physics/CollisionResolver.hpp"
#include "player/Player.hpp"
#include <glm/glm.hpp>

World::~World() = default;

World::World(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	player_ = std::make_unique<Player>();
}

void World::loadMap(std::unique_ptr<Map> map) {
	currentMap_ = std::move(map);
	currentMap_->start();

	player_->position = currentMap_->getPlayerSpawnPosition();
	player_->velocity = glm::vec2(0, 0);

	const float stageWidth  = currentMap_->getPixelWidth();
	const float stageHeight = currentMap_->getPixelHeight();

	collision_ = std::make_unique<CollisionResolver>(*player_, *currentMap_,
	                                                 stageWidth, stageHeight);
	camera_    = std::make_unique<Camera>(*player_, windowWidth_, windowHeight_,
	                                      stageWidth, stageHeight);
}

Player&            World::player()    { return *player_; }
Map&               World::map()       { return *currentMap_; }
Camera&            World::camera()    { return *camera_; }
CollisionResolver& World::collision() { return *collision_; }
int                World::windowWidth()  const { return windowWidth_; }
int                World::windowHeight() const { return windowHeight_; }
