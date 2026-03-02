#include "AnimatedSprite.hpp"
#include <stdexcept>

void AnimatedSprite::addAnimation(const std::string& name, Animation anim) {
	animations_[name] = std::move(anim);
}

void AnimatedSprite::play(const std::string& name) {
	if (currentAnim_ == name) return;

	currentAnim_  = name;
	currentFrame_ = 0;
	timer_        = 0.0f;
}

void AnimatedSprite::update(float deltaTime) {
	if (currentAnim_.empty()) return;

	auto& anim = animations_.at(currentAnim_);
	if (anim.frames.empty()) return;

	timer_ += deltaTime;
	if (timer_ >= anim.frameDuration) {
		timer_ -= anim.frameDuration;
		currentFrame_ = (currentFrame_ + 1) % static_cast<int>(anim.frames.size());
	}
}

const Texture& AnimatedSprite::currentTexture() const {
	if (currentAnim_.empty()) {
		throw std::runtime_error("AnimatedSprite: no animation is playing");
	}
	const auto& anim = animations_.at(currentAnim_);
	return *anim.frames[currentFrame_];
}
