#ifndef NEXT_MAP_ACTION_HPP
#define NEXT_MAP_ACTION_HPP

#include "../StoryContext.hpp"
#include "Action.hpp"

// 次のマップへの遷移をリクエストする。start() 時点で即座に完了する。
// GameScene が ctx.mapTransitionRequested を見て loadMap() を呼ぶ。
class NextMapAction : public Action {
public:
	void start(StoryContext& ctx) override {
		ctx.mapTransitionRequested = true;
		done_                      = true;
	}
	void update(float, StoryContext&) override {}
	bool isFinished() const override { return done_; }

private:
	bool done_ = false;
};

#endif // NEXT_MAP_ACTION_HPP
