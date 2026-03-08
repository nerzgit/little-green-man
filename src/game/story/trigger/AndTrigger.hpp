#ifndef AND_TRIGGER_HPP
#define AND_TRIGGER_HPP

#include "Trigger.hpp"
#include <memory>
#include <vector>

// 複数のトリガーをまとめ、全てが pass したときだけ発火する。
// 個々のトリガーは通常通り定義し、JSON では "type": "and" で入れ子にする。
class AndTrigger : public Trigger {
public:
	explicit AndTrigger(std::vector<std::unique_ptr<Trigger>> conditions)
	    : conditions_(std::move(conditions)) {
	}

	bool check(const StoryContext& ctx) const override {
		for (const auto& t : conditions_) {
			if (!t->check(ctx)) return false;
		}
		return true;
	}

private:
	std::vector<std::unique_ptr<Trigger>> conditions_;
};

#endif // AND_TRIGGER_HPP
