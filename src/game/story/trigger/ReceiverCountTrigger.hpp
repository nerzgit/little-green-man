#ifndef RECEIVER_COUNT_TRIGGER_HPP
#define RECEIVER_COUNT_TRIGGER_HPP

#include "Trigger.hpp"
#include "../StoryContext.hpp"

// activatedReceiverCount >= threshold のとき発火
class ReceiverCountTrigger : public Trigger {
public:
	explicit ReceiverCountTrigger(int threshold) : threshold_(threshold) {}

	bool check(const StoryContext& ctx) const override {
		return ctx.activatedReceiverCount >= threshold_;
	}

private:
	int threshold_;
};

#endif // RECEIVER_COUNT_TRIGGER_HPP
