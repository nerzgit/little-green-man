#include "ZoneTrigger.hpp"

#include "../../player/Player.hpp"
#include "../StoryContext.hpp"

bool ZoneTrigger::check(const StoryContext& ctx) const {
	const glm::vec2& pos = ctx.player.position;
	return pos.x >= x_ && pos.x <= x_ + w_ && pos.y >= y_ && pos.y <= y_ + h_;
}
