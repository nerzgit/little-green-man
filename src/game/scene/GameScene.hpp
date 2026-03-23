// =============================================================================
//  GameScene.hpp
//
//  ゲームプレイ中のシーン。SceneManager から毎フレーム
//  update() / render() が呼ばれ、ゲームループを回す。
//
//  公開インターフェース:
//    update()  ─ 入力・ストーリー・物理・AI を処理し、必要なら別シーンへ遷移
//    render()  ─ 5レイヤーを順番に描画
//
//  内部設計:
//    World と StoryManager の 2 本柱で構成される。
//    World はゲームオブジェクトの寿命を管理し、
//    StoryManager は JSON 定義の演出キューを担当する。
//    両者の連携は GameScene だけが知っており、互いは直接参照しない。
// =============================================================================

#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../World.hpp"
#include "../story/StoryManager.hpp"
#include "Scene.hpp"

class GameScene : public Scene {
public:
	GameScene(int windowWidth, int windowHeight);

	// Scene インターフェース
	void update(float deltaTime, SceneManager& sm) override;
	void render(Renderer& renderer) override;

private:
	// マップを差し替え、ストーリーを再ロードする。
	// ゲーム開始時・NextMap イベント発火時の両方で呼ばれる。
	void loadMap(std::unique_ptr<class Map> newMap);

	// ストーリー演出を更新する。NextMap が発火した場合 true を返す。
	// true のとき呼び出し元は物理/AI 更新をスキップすること。
	bool updateStory(float deltaTime);

	// プレイヤー移動 → 衝突解決 → マップ押し戻しの順で物理を確定する。
	void updatePhysics(float deltaTime);

	World        world_;         // プレイヤー / マップ / カメラ / 衝突解決器
	StoryManager storyManager_;  // JSON 定義のイベント駆動演出
};

#endif // GAME_SCENE_HPP
