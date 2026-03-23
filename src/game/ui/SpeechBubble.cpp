#include "SpeechBubble.hpp"

#include "../../engine/graphics/Texture.hpp"
#include "../../engine/ui/DialogueBox.hpp"
#include "../../engine/font/FontManager.hpp"

// 静的メンバの定義
std::unique_ptr<DialogueBox> SpeechBubble::dialogueBox_;
std::queue<std::string>      SpeechBubble::messageQueue_;

void SpeechBubble::init(int windowWidth, int windowHeight) {
	dialogueBox_ = std::make_unique<DialogueBox>(windowWidth, windowHeight);
}

void SpeechBubble::enqueue(const std::string& utf8Text) {
	messageQueue_.push(utf8Text);
}

void SpeechBubble::update(float deltaTime) {
	if (!dialogueBox_) return;

	dialogueBox_->update(deltaTime);

	// 表示中のメッセージが終わったらキューから次を取り出す
	if (!dialogueBox_->isVisible() && !messageQueue_.empty()) {
		dialogueBox_->open(messageQueue_.front());
		messageQueue_.pop();
	}
}

void SpeechBubble::draw(Renderer& renderer) {
	if (!dialogueBox_) return;
	dialogueBox_->draw(renderer, FontManager::getInstance().getDefaultFont());
}

SpeechBubble::State SpeechBubble::getState() {
	if (!dialogueBox_) return State::Idle;
	return (dialogueBox_->isVisible() || !messageQueue_.empty())
	         ? State::Blocking
	         : State::Idle;
}
