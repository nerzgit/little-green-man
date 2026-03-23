#ifndef STORY_MANAGER_HPP
#define STORY_MANAGER_HPP

#include "ActionQueue.hpp"
#include "StoryEvent.hpp"
#include <string>
#include <unordered_set>
#include <vector>

class Player;

// ストーリーシステムの中心。JSON を読み込み、毎フレーム
// トリガーを監視してアクションを実行する。
//
// GameScene は load() / update() / isBlocking() / captureEvent() だけ知っていればよい。
class StoryManager {
public:
	enum class Event { None, NextMap };

	// マップ切り替え時に呼ぶ。JSONがなければイベントをクリアするだけ。
	void load(const std::string& jsonPath);

	// 毎フレーム GameScene から呼ぶ
	void update(float dt, Player& player);

	// true の間はプレイヤー操作をブロックすること
	bool isBlocking() const { return !actionQueue_.isEmpty(); }

	// 発生中のイベントを取り出す。呼び出すと None にリセットされる
	Event captureEvent();

private:
	std::vector<StoryEvent>         events_;
	ActionQueue                     actionQueue_;
	std::unordered_set<std::string> flags_;
	Event                           pendingEvent_ = Event::None;
};

#endif // STORY_MANAGER_HPP
