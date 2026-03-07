#include "GameScene.hpp"

#include "../../engine/graphics/FontAtlas.hpp"
#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "../../engine/ui/DialogueBox.hpp"
#include "../camera/Camera.hpp"
#include "../map/world1/Map1_1.hpp"
#include "../physics/CollisionResolver.hpp"
#include "../player/Player.hpp"
#include "GameOverScene.hpp"
#include "SceneManager.hpp"
#include "TitleScreen.hpp"

GameScene::GameScene(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	font_ = std::make_unique<FontAtlas>("assets/fonts/PixelMplus12-Regular.ttf",
	                                    24.0f, 2048);
	dialogueBox_ = std::make_unique<DialogueBox>(windowWidth, windowHeight);

	// Player は仮位置で生成し、loadMap 内で正しい位置に移動する
	player_ = std::make_unique<Player>(glm::vec2(0, 0));

	loadMap(std::make_unique<Map1_1>());
}

GameScene::~GameScene() = default;

void GameScene::loadMap(std::unique_ptr<Map> newMap) {
	currentMap_ = std::move(newMap);
	currentMap_->start();

	// プレイヤーをスポーン位置に移動
	player_->position = currentMap_->getPlayerSpawnPosition();
	player_->velocity = glm::vec2(0, 0);

	const float stageWidth  = currentMap_->getPixelWidth();
	const float stageHeight = currentMap_->getPixelHeight();

	collision_ = std::make_unique<CollisionResolver>(*player_, *currentMap_,
	                                                 stageWidth, stageHeight);
	camera_    = std::make_unique<Camera>(*player_, windowWidth_, windowHeight_,
	                                      stageWidth, stageHeight);

	storyManager_.load(currentMap_->getStoryPath());
}

void GameScene::update(float deltaTime, SceneManager& sm) {
	// ESC でタイトルへ戻る
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		sm.switchTo(std::make_unique<TitleScreen>(sm.getWindowWidth(),
		                                          sm.getWindowHeight()));
		return;
	}

	dialogueBox_->update(deltaTime);

	StoryContext ctx{*player_, *dialogueBox_, storyFlags_};
	storyManager_.update(deltaTime, ctx);

	// ストーリー実行中（会話・演出）はプレイヤー操作をブロック
	if (storyManager_.isBlocking()) return;

	player_->update(deltaTime);
	collision_->resolve();

	switch (currentMap_->update(deltaTime, *player_)) {
	case MapEvent::NextMap:
		if (auto next = currentMap_->createNextMap()) {
			loadMap(std::move(next));
		}
		return;
	case MapEvent::PlayerDead:
		sm.switchTo(std::make_unique<GameOverScene>(sm.getWindowWidth(),
		                                            sm.getWindowHeight()));
		return;
	case MapEvent::None: break;
	}
}

void GameScene::render(Renderer& renderer) {
	camera_->apply(renderer);
	renderer.clear();

	if (currentMap_) {
		currentMap_->drawBackground(renderer);
		currentMap_->drawEnemy(renderer);
	}

	if (player_->active) {
		player_->draw(renderer);
	}

	if (currentMap_) {
		currentMap_->drawForeground(renderer);
	}

	// HUD層: 会話ボックスはワールド描画の後に重ねる
	dialogueBox_->draw(renderer, *font_);
}
