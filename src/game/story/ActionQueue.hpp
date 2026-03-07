#ifndef ACTION_QUEUE_HPP
#define ACTION_QUEUE_HPP

#include "Action.hpp"
#include <deque>
#include <memory>
#include <vector>

struct StoryContext;

// アクションを順番に実行するキュー。
// 先頭のアクションが isFinished() になるまで次に進まない。
class ActionQueue {
public:
	void enqueue(std::vector<std::unique_ptr<Action>> actions);
	void update(float dt, StoryContext& ctx);
	void clear();
	bool isEmpty() const { return queue_.empty(); }

private:
	std::deque<std::unique_ptr<Action>> queue_;
	bool                                started_ = false;
};

#endif // ACTION_QUEUE_HPP
