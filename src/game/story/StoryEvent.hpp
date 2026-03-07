#ifndef STORY_EVENT_HPP
#define STORY_EVENT_HPP

#include "action/Action.hpp"
#include "trigger/Trigger.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

// アクションを毎回フレッシュなインスタンスで生成するためのファクトリ型。
// once=false のイベント（繰り返し会話など）で同じアクションを再実行できる。
using ActionFactory = std::function<std::unique_ptr<Action>()>;

struct StoryEvent {
	std::string              id;
	bool                     once  = true;
	bool                     fired = false;
	std::unique_ptr<Trigger> trigger;
	std::vector<ActionFactory> actionFactories;
};

#endif // STORY_EVENT_HPP
