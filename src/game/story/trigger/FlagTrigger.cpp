#include "FlagTrigger.hpp"

#include "../StoryContext.hpp"

bool FlagTrigger::check(const StoryContext& ctx) const {
	return ctx.flags.count(flag_) > 0;
}
