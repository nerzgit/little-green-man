#ifndef SET_FLAG_ACTION_HPP
#define SET_FLAG_ACTION_HPP

#include "Action.hpp"
#include <string>

// ストーリーフラグを立てる。start() 時点で即座に完了する。
class SetFlagAction : public Action {
public:
	explicit SetFlagAction(std::string flag) : flag_(std::move(flag)) {}

	void start(StoryContext& ctx) override;
	void update(float, StoryContext&) override {}
	bool isFinished() const override { return done_; }

private:
	std::string flag_;
	bool        done_ = false;
};

#endif // SET_FLAG_ACTION_HPP
