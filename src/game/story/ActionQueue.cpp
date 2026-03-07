#include "ActionQueue.hpp"

#include "StoryContext.hpp"

void ActionQueue::enqueue(std::vector<std::unique_ptr<Action>> actions) {
	for (auto& a : actions) {
		queue_.push_back(std::move(a));
	}
}

void ActionQueue::update(float dt, StoryContext& ctx) {
	// SetFlagAction のように即座に完了するアクションは同一フレームで連続実行する
	while (!queue_.empty()) {
		auto& front = queue_.front();

		if (!started_) {
			front->start(ctx);
			started_ = true;
		}

		front->update(dt, ctx);

		if (front->isFinished()) {
			queue_.pop_front();
			started_ = false;
		} else {
			break; // このアクションの完了を待つ
		}
	}
}

void ActionQueue::clear() {
	queue_.clear();
	started_ = false;
}
