#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "Texture.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Animation {
	std::vector<std::shared_ptr<Texture>> frames;
	float                                 frameDuration; // 秒
};

class AnimatedSprite {
public:
	void addAnimation(const std::string& name, Animation anim);
	void play(const std::string& name);
	void update(float deltaTime);

	const Texture& currentTexture() const;

private:
	std::unordered_map<std::string, Animation> animations_;
	std::string                                currentAnim_;
	int                                        currentFrame_ = 0;
	float                                      timer_        = 0.0f;
};

#endif // ANIMATED_SPRITE_HPP
