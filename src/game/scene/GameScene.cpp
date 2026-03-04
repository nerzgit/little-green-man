#include "GameScene.hpp"

#include "../../engine/graphics/FontAtlas.hpp"
#include "../../engine/graphics/Renderer.hpp"  // glad を GLFW より先に
#include "../../engine/input/InputManager.hpp" // GLFW/glfw3.h
#include "../camera/Camera.hpp"
#include "../enemy/Enemy.hpp"
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

	// Player は仮位置で生成し、loadMap 内で正しい位置に移動する
	player_ = std::make_unique<Player>(glm::vec2(0, 0));

	loadMap(std::make_unique<Map1_1>());
}

GameScene::~GameScene() = default;

void GameScene::loadMap(std::unique_ptr<Map> newMap) {
	currentMap_ = std::move(newMap);
	currentMap_->onStart();

	// プレイヤーをスポーン位置に移動
	player_->position = currentMap_->getPlayerSpawnPosition();
	player_->velocity = glm::vec2(0, 0);

	const float stageWidth  = currentMap_->getPixelWidth();
	const float stageHeight = currentMap_->getPixelHeight();

	collision_ = std::make_unique<CollisionResolver>(*player_, *currentMap_,
	                                                 stageWidth, stageHeight);
	camera_    = std::make_unique<Camera>(*player_, windowWidth_, windowHeight_,
	                                      stageWidth, stageHeight);

	enemies_.clear();
	for (const auto& pos : currentMap_->getEnemySpawnPositions()) {
		enemies_.push_back(std::make_unique<Enemy>(pos, 150.0f));
	}
}

void GameScene::update(float deltaTime, SceneManager& sm) {
	// ESC でタイトルへ戻る
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		sm.switchTo(std::make_unique<TitleScreen>(sm.getWindowWidth(),
		                                          sm.getWindowHeight()));
		return;
	}

	player_->update(deltaTime);
	collision_->resolve();

	// トリガータイルに乗ったら次のマップへ
	if (currentMap_->isTriggerAt(player_->position.x, player_->position.y)) {
		auto nextMap = currentMap_->createNextMap();
		if (nextMap) {
			loadMap(std::move(nextMap));
			return;
		}
	}

	// 敵の更新とプレイヤーとの当たり判定
	for (auto& enemy : enemies_) {
		enemy->update(deltaTime);

		if (player_->intersects(*enemy)) {
			sm.switchTo(std::make_unique<GameOverScene>(sm.getWindowWidth(),
			                                            sm.getWindowHeight()));
			return;
		}
	}
}

void GameScene::render(Renderer& renderer) {
	camera_->apply(renderer);
	renderer.clear();

	if (currentMap_) {
		currentMap_->onDraw(renderer);
	}

	for (auto& enemy : enemies_) {
		enemy->draw(renderer);
	}

	if (player_->active) {
		player_->draw(renderer);
	}

	renderer.drawTextHUD(*font_,
	                     "やあ 僕はnerzgitによってうみだされたんだけど これか"
	                     "らどうなっていくのかまだわからないんだ",
	                     glm::vec2(8, 20), 0.5f);
}
