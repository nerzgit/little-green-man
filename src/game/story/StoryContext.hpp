#ifndef STORY_CONTEXT_HPP
#define STORY_CONTEXT_HPP

#include <string>
#include <unordered_set>

class DialogueBox;
class Player;

// StoryManager が毎フレーム受け取るゲーム状態へのビュー。
// Trigger/Action はこれ経由でゲームオブジェクトにアクセスする。
struct StoryContext {
	Player&                          player;
	DialogueBox&                     dialogueBox;
	std::unordered_set<std::string>& flags; // ストーリーフラグ（set_flag で立てる）
};

#endif // STORY_CONTEXT_HPP
