#ifndef STORY_MANAGER_HPP
#define STORY_MANAGER_HPP

#include "ActionQueue.hpp"
#include "StoryContext.hpp"
#include "StoryEvent.hpp"
#include <string>
#include <vector>

// ストーリーシステムの中心。JSON を読み込み、毎フレーム
// トリガーを監視してアクションを実行する。
//
// GameScene は storyManager_->update() と isBlocking() だけ知っていればよい。
class StoryManager {
public:
	// マップ切り替え時に呼ぶ。JSONがなければイベントをクリアするだけ。
	void load(const std::string& jsonPath);

	// 毎フレーム GameScene から呼ぶ
	void update(float dt, StoryContext& ctx);

	// true の間はプレイヤー操作をブロックすること
	bool isBlocking() const { return !actionQueue_.isEmpty(); }

private:
	std::vector<StoryEvent> events_;
	ActionQueue             actionQueue_;

	// JSON解析はStoryManager.cppの無名名前空間に閉じ込め、ヘッダを汚さない
};

#endif // STORY_MANAGER_HPP
