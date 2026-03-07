#ifndef WAIT_ACTION_HPP
#define WAIT_ACTION_HPP

#include "Action.hpp"

// N 秒間待つ（演出・間合いの調整用）。外部依存なし。
class WaitAction : public Action {
public:
	explicit WaitAction(float seconds) : duration_(seconds) {}

	void start(StoryContext&) override { elapsed_ = 0.f; }
	void update(float dt, StoryContext&) override { elapsed_ += dt; }
	bool isFinished() const override { return elapsed_ >= duration_; }

private:
	float duration_;
	float elapsed_ = 0.f;
};

#endif // WAIT_ACTION_HPP
