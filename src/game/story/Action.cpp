#include "Action.hpp"

#include "../../engine/ui/DialogueBox.hpp"
#include "StoryContext.hpp"

// --- DialogueAction ---

void DialogueAction::start(StoryContext& ctx) {
	pageIndex_ = 0;
	finished_  = lines_.empty();
	if (!finished_) {
		ctx.dialogueBox.open(lines_[0]);
	}
}

void DialogueAction::update(float, StoryContext& ctx) {
	if (finished_) return;

	// DialogueBox が閉じたらページを進める
	if (!ctx.dialogueBox.isVisible()) {
		++pageIndex_;
		if (pageIndex_ < static_cast<int>(lines_.size())) {
			ctx.dialogueBox.open(lines_[pageIndex_]);
		} else {
			finished_ = true;
		}
	}
}

// --- SetFlagAction ---

void SetFlagAction::start(StoryContext& ctx) {
	ctx.flags.insert(flag_);
	done_ = true;
}
