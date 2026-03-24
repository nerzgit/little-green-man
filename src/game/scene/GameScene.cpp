// =============================================================================
//  GameScene.cpp
//
//  ゲームプレイ中の「生きているシーン」。
//
//  所有関係:
//      GameScene
//        ├── World               ─ プレイヤー / マップ / カメラ / 衝突解決器を束ねる器
//        │     ├── Player        ─ 入力受付・速度積分・スプライト
//        │     ├── Map           ─ タイル / 敵 / ギミック / 次マップへのポインタ
//        │     ├── Camera        ─ プレイヤー追従・ビュー行列
//        │     └── CollisionResolver ─ AABB ベースの押し戻し
//        └── StoryManager        ─ JSON イベント駆動の演出キュー
//
//  毎フレームの処理順 (update):
//      1. ESC → 即終了
//      2. ストーリー更新  → NextMap イベントが来たらマップ差し替えて return
//      3. SpeechBubble がブロック中 → 物理/AI を止めて return
//      4. 物理更新        → プレイヤー移動 → 衝突解決
//      5. マップ更新      → 敵 AI / ギミック → PlayerDead なら GameOverScene へ
//
//  描画レイヤー順 (render):
//      Background  →  YSort(プレイヤー+敵)  →  Foreground
// =============================================================================

#include "GameScene.hpp"

#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "../camera/Camera.hpp"
#include "../enemy/Enemy.hpp"
#include "../enemy/EnemySystem.hpp"
#include "../map/Map.hpp"
#include "../physics/CollisionResolver.hpp"
#include "../player/Player.hpp"
#include "../ui/SpeechBubble.hpp"
#include "GameOverScene.hpp"
#include "SceneManager.hpp"
#include <algorithm>
#include <functional>
#include <vector>

// -----------------------------------------------------------------------------
//  構築
// -----------------------------------------------------------------------------

GameScene::GameScene(int windowWidth, int windowHeight)
    : world_(windowWidth, windowHeight) {
	SpeechBubble::init(windowWidth, windowHeight);
	loadMap(Map::load("assets/maps/map1_1.json"));
}

// -----------------------------------------------------------------------------
//  マップ差し替え
// -----------------------------------------------------------------------------

void GameScene::loadMap(std::unique_ptr<Map> newMap) {
	world_.loadMap(std::move(newMap));
	storyManager_.load(world_.map().getStoryPath());
}

// -----------------------------------------------------------------------------
//  ストーリー更新
// -----------------------------------------------------------------------------

bool GameScene::updateStory(float deltaTime) {
	SpeechBubble::update(deltaTime);
	storyManager_.update(deltaTime, world_.player());

	if (storyManager_.captureEvent() == StoryManager::Event::NextMap) {
		if (auto next = world_.map().createNextMap())
			loadMap(std::move(next));
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
//  物理更新
// -----------------------------------------------------------------------------

void GameScene::updatePhysics(float deltaTime) {
	world_.player().update(deltaTime);
	world_.collision().resolve();
}

// -----------------------------------------------------------------------------
//  メイン更新ループ (1フレーム)
// -----------------------------------------------------------------------------

void GameScene::update(float deltaTime, SceneManager& sm) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE))
		std::exit(0);

	if (updateStory(deltaTime))
		return;

	if (SpeechBubble::getState() == SpeechBubble::State::Blocking)
		return;

	updatePhysics(deltaTime);

	if (world_.update(deltaTime))
		sm.switchTo(std::make_unique<GameOverScene>(sm.getWindowWidth(),
		                                            sm.getWindowHeight()));
}

// -----------------------------------------------------------------------------
//  描画 (1フレーム)
// -----------------------------------------------------------------------------

void GameScene::render(Renderer& renderer) {
	world_.camera().apply(renderer);
	renderer.clear();

	world_.map().draw(renderer, DrawLayer::Background);

	// Y-sort: プレイヤーと敵を Y 座標順に描画して奥行きを表現する
	using DrawFn = std::function<void(Renderer&)>;
	std::vector<std::pair<float, DrawFn>> drawList;

	for (const auto& e : world_.enemySystem().enemies())
		drawList.push_back({e->position.y, [ep = e.get()](Renderer& r) { ep->draw(r); }});

	if (world_.player().active)
		drawList.push_back({world_.player().position.y,
		                    [&](Renderer& r) { world_.player().draw(r); }});

	std::sort(drawList.begin(), drawList.end(),
	          [](const auto& a, const auto& b) { return a.first < b.first; });

	for (auto& [y, draw] : drawList)
		draw(renderer);

	world_.map().draw(renderer, DrawLayer::Foreground);

	SpeechBubble::draw(renderer); // HUD層
}
