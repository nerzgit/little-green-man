#include "SetFlagAction.hpp"

#include "../StoryContext.hpp"

void SetFlagAction::start(StoryContext& ctx) {
	ctx.flags.insert(flag_);
	done_ = true;
}
