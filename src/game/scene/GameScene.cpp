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
		// 一旦強制終了
		std::exit(0);
		// sm.switchTo(std::make_unique<TitleScreen>(sm.getWindowWidth(),
		//                                           sm.getWindowHeight()));
		return;
	}

	// 会話ボックスの更新（文字送りや閉じる操作の受付）
	dialogueBox_->update(deltaTime);

	// ストーリーイベントの更新（トリガーチェックとアクション実行）
	StoryContext ctx{*player_, *dialogueBox_, storyFlags_,
	                 currentMap_->getActivatedReceiverCount()};
	storyManager_.update(deltaTime, ctx);

	// マップ遷移リクエストがあれば次のマップへ
	if (ctx.mapTransitionRequested) {
		if (auto next = currentMap_->createNextMap()) {
			loadMap(std::move(next));
		}
		return;
	}

	// ストーリー実行中（会話・演出）はプレイヤー操作をブロック
	if (storyManager_.isBlocking())
		return;

	// プレイヤーの更新とマップの衝突判定
	player_->update(deltaTime);
	collision_->resolve();
	currentMap_->resolvePush(*player_);

	// マップの更新（敵の移動やマップイベントの発火）
	switch (currentMap_->update(deltaTime, *player_)) {
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
		currentMap_->drawLight(renderer);
	}

	if (player_->active) {
		player_->draw(renderer);
	}

	if (currentMap_) {
		currentMap_->drawForeground(renderer);
		currentMap_->drawLight(renderer);
	}

	// オーバーレイ: 会話ボックスより下、ゲーム画面より上
	if (currentMap_)
		currentMap_->drawLightOverlay(renderer, windowWidth_, windowHeight_);

	// パララックスオーバーレイ
	if (currentMap_)
		currentMap_->drawParallaxOverlay(renderer, windowWidth_, windowHeight_,
		                                 camera_->getPosition().x);

	// 最前面: ゲーム全体の色味オーバーレイ
	if (currentMap_)
		currentMap_->drawColorOverlay(renderer, windowWidth_, windowHeight_);

	// HUD層: 会話ボックスはワールド描画の後に重ねる
	dialogueBox_->draw(renderer, *font_);
}
