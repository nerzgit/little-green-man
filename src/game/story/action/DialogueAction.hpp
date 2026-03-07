#ifndef DIALOGUE_ACTION_HPP
#define DIALOGUE_ACTION_HPP

#include "Action.hpp"
#include <string>
#include <vector>

// 複数ページの会話を順番に表示する。
// 各ページはユーザーが Z / Space / Enter を押すと閉じ、次のページへ進む。
class DialogueAction : public Action {
public:
	explicit DialogueAction(std::vector<std::string> lines)
	    : lines_(std::move(lines)) {
	}

	void start(StoryContext& ctx) override;
	void update(float dt, StoryContext& ctx) override;
	bool isFinished() const override { return finished_; }

private:
	std::vector<std::string> lines_;
	int                      pageIndex_ = 0;
	bool                     finished_  = false;
};

#endif // DIALOGUE_ACTION_HPP
