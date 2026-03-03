#include "GameManager.hpp"

#include "../engine/graphics/Renderer.hpp"
#include "camera/Camera.hpp"
#include "map/world1/Map1_1.hpp"
#include "physics/CollisionResolver.hpp"
#include "player/Player.hpp"

GameManager::GameManager(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	currentMap_ = std::make_unique<Map1_1>();
	currentMap_->onStart();

	player_ = std::make_unique<Player>(
	  glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f));

	const float stageWidth  = currentMap_->getPixelWidth();
	const float stageHeight = currentMap_->getPixelHeight();

	collision_ = std::make_unique<CollisionResolver>(*player_, *currentMap_,
	                                                 stageWidth, stageHeight);

	camera_ = std::make_unique<Camera>(*player_, windowWidth, windowHeight,
	                                   stageWidth, stageHeight);
}

GameManager::~GameManager() = default;

void GameManager::update(float deltaTime) {
	player_->update(deltaTime);
	collision_->resolve();
}

void GameManager::render(Renderer& renderer) {
	camera_->apply(renderer);
	renderer.clear();

	if (currentMap_) {
		currentMap_->onDraw(renderer);
	}

	if (player_->active) {
		player_->draw(renderer);
	}
}
