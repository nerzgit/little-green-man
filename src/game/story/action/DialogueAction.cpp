#include "DialogueAction.hpp"

#include "../../../game/ui/SpeechBubble.hpp"
#include "../StoryContext.hpp"

void DialogueAction::start(StoryContext&) {
	finished_ = lines_.empty();
	for (const auto& line : lines_) {
		SpeechBubble::enqueue(line);
	}
}

void DialogueAction::update(float, StoryContext&) {
	if (!finished_ && SpeechBubble::getState() == SpeechBubble::State::Idle) {
		finished_ = true;
	}
}
