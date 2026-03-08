#ifndef FLAG_TRIGGER_HPP
#define FLAG_TRIGGER_HPP

#include "Trigger.hpp"
#include <string>

// 指定したストーリーフラグが立っているときに発火。
// SetFlagAction と組み合わせてイベントの順序依存を表現する。
class FlagTrigger : public Trigger {
public:
	explicit FlagTrigger(std::string flag) : flag_(std::move(flag)) {}

	bool check(const StoryContext& ctx) const override;

private:
	std::string flag_;
};

#endif // FLAG_TRIGGER_HPP
