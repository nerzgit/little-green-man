#ifndef STORY_CONTEXT_HPP
#define STORY_CONTEXT_HPP

#include <string>
#include <unordered_set>

class Player;

// StoryManager が毎フレーム受け取るゲーム状態へのビュー。
// Trigger/Action はこれ経由でゲームオブジェクトにアクセスする。
struct StoryContext {
	Player&                          player;
	std::unordered_set<std::string>& flags;       // ストーリーフラグ（set_flag で立てる）
	bool mapTransitionRequested = false;          // next_map アクションが立てる
};

#endif // STORY_CONTEXT_HPP
