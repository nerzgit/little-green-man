#include "Trigger.hpp"

#include "../../game/player/Player.hpp"
#include "StoryContext.hpp"

bool ZoneTrigger::check(const StoryContext& ctx) const {
	const glm::vec2& pos = ctx.player.position;
	return pos.x >= x_ && pos.x <= x_ + w_ && pos.y >= y_ && pos.y <= y_ + h_;
}

bool MapLoadTrigger::check(const StoryContext&) const {
	if (armed_) {
		armed_ = false;
		return true;
	}
	return false;
}
