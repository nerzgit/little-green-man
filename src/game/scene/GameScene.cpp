#include "GameScene.hpp"

#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "../camera/Camera.hpp"
#include "../map/world1/Map1_1.hpp"
#include "../physics/CollisionResolver.hpp"
#include "../player/Player.hpp"
#include "../ui/SpeechBubble.hpp"
#include "GameOverScene.hpp"
#include "SceneManager.hpp"
#include <algorithm>

GameScene::GameScene(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	SpeechBubble::init(windowWidth, windowHeight);

	/**
	 * プレイヤーの初期座標
	 */
	player_ = std::make_unique<Player>();

	/**
	 * 最初のマップをロード
	 */
	loadMap(std::make_unique<Map1_1>());
}

GameScene::~GameScene() = default;

void GameScene::loadMap(std::unique_ptr<Map> newMap) {
	/**
	 * マップを切り替える際の処理
	 * - マップの start() を呼び出す
	 */
	currentMap_ = std::move(newMap);
	currentMap_->start();

	/**
	 * プレイヤーをスポーン位置に移動
	 */
	player_->position = currentMap_->getPlayerSpawnPosition();
	player_->velocity = glm::vec2(0, 0);

	const float stageWidth  = currentMap_->getPixelWidth();
	const float stageHeight = currentMap_->getPixelHeight();

	/**
	 * 衝突判定計算を実行する CollisionResolver を生成
	 */
	collision_ = std::make_unique<CollisionResolver>(*player_, *currentMap_,
	                                                 stageWidth, stageHeight);

	/**
	 * カメラを生成
	 */
	camera_ = std::make_unique<Camera>(*player_, windowWidth_, windowHeight_,
	                                   stageWidth, stageHeight);

	/**
	 * ストーリーは各マップごとのイベントや会話を管理するため、マップ切り替えのタイミングでロードし直す
	 */
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
	SpeechBubble::update(deltaTime);

	/**
	 * ストーリーの更新
	 * - 会話の開始・終了
	 * - 演出のトリガー
	 * - マップ遷移のリクエスト
	 * これ以外に増えたら都度追加する
	 */
	StoryContext storyCtx{*player_, storyFlags_,
	                      currentMap_->getActivatedReceiverCount()};
	storyManager_.update(deltaTime, storyCtx);

	/**
	 * マップ遷移のリクエストがあったら次のマップに切り替える
	 */
	if (storyCtx.mapTransitionRequested) {
		if (auto next = currentMap_->createNextMap()) {
			loadMap(std::move(next));
		}
		return;
	}

	/**
	 * メッセージ表示中はプレイヤー操作をブロック
	 */
	if (SpeechBubble::isBlocking())
		return;

	player_->update(deltaTime);

	collision_->resolve();

	currentMap_->resolvePush(*player_);

	/**
	 * マップの update() は MapEvent を返す。これが PlayerDead
	 * ならゲームオーバーに遷移する。
	 */
	switch (currentMap_->update(deltaTime, *player_)) {
	case MapEvent::PlayerDead:
		sm.switchTo(std::make_unique<GameOverScene>(sm.getWindowWidth(),
		                                            sm.getWindowHeight()));
		return;
	case MapEvent::None: break;
	}
}

void GameScene::drawYSorted(Renderer& renderer) {
	std::vector<Drawable> drawables;
	currentMap_->collectDrawables(drawables);
	if (player_->active)
		drawables.push_back(
		  {player_->position.y, [&](Renderer& r) { player_->draw(r); }});
	std::sort(
	  drawables.begin(), drawables.end(),
	  [](const Drawable& a, const Drawable& b) { return a.sortY < b.sortY; });
	for (auto& d : drawables)
		d.draw(renderer);
}

void GameScene::render(Renderer& renderer) {
	camera_->apply(renderer);
	renderer.clear();

	currentMap_->draw(renderer, DrawLayer::Background);
	drawYSorted(renderer);
	currentMap_->draw(renderer, DrawLayer::Foreground);
	currentMap_->drawEffects(renderer, windowWidth_, windowHeight_,
	                         camera_->getPosition().x, player_->position.y);

	// HUD層: ワールド描画の後に重ねる
	SpeechBubble::draw(renderer);
}
