#include "GameScene.hpp"

#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "../camera/Camera.hpp"
#include "../map/Map.hpp"
#include "../map/world1/Map1_1.hpp"
#include "../physics/CollisionResolver.hpp"
#include "../player/Player.hpp"
#include "../rendering/YSortRenderer.hpp"
#include "../story/StoryContext.hpp"
#include "../ui/SpeechBubble.hpp"
#include "GameOverScene.hpp"
#include "SceneManager.hpp"

GameScene::GameScene(int windowWidth, int windowHeight)
    : world_(windowWidth, windowHeight) {
	SpeechBubble::init(windowWidth, windowHeight);
	loadMap(std::make_unique<Map1_1>());
}

void GameScene::loadMap(std::unique_ptr<Map> newMap) {
	world_.loadMap(std::move(newMap));
	// ストーリーはマップごとに管理されるため、切り替えのたびにロードし直す
	storyManager_.load(world_.map().getStoryPath());
}

void GameScene::update(float deltaTime, SceneManager& sm) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE))
		std::exit(0);

	SpeechBubble::update(deltaTime);

	StoryContext storyCtx{world_.player(), storyFlags_,
	                      world_.map().getActivatedReceiverCount()};
	storyManager_.update(deltaTime, storyCtx);

	if (storyCtx.mapTransitionRequested) {
		if (auto next = world_.map().createNextMap())
			loadMap(std::move(next));
		return;
	}

	if (SpeechBubble::isBlocking())
		return;

	world_.player().update(deltaTime);
	world_.collision().resolve();
	world_.map().resolvePush(world_.player());

	switch (world_.map().update(deltaTime, world_.player())) {
	case MapEvent::PlayerDead:
		sm.switchTo(std::make_unique<GameOverScene>(sm.getWindowWidth(),
		                                            sm.getWindowHeight()));
		return;
	case MapEvent::None: break;
	}
}

void GameScene::render(Renderer& renderer) {
	world_.camera().apply(renderer);
	renderer.clear();

	world_.map().draw(renderer, DrawLayer::Background);
	YSortRenderer::draw(world_, renderer);
	world_.map().draw(renderer, DrawLayer::Foreground);
	world_.map().drawEffects(renderer, world_);

	SpeechBubble::draw(renderer); // HUD層
}
