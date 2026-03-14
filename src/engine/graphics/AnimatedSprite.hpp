#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "Texture.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// 1フレーム分のテクスチャとUV範囲。
// 個別ファイルの場合 uvMin={0,0}, uvMax={1,1}。
// アトラスの場合はフレームに対応するUVを指定する。
struct SpriteFrame {
	std::shared_ptr<Texture> texture;
	glm::vec2                uvMin = {0.0f, 0.0f};
	glm::vec2                uvMax = {1.0f, 1.0f};
};

struct Animation {
	std::vector<SpriteFrame> frames;
	float                    frameDuration; // 秒
};

class AnimatedSprite {
public:
	void addAnimation(const std::string& name, Animation anim);
	void play(const std::string& name);
	void update(float deltaTime);

	const SpriteFrame& currentFrame() const;

private:
	std::unordered_map<std::string, Animation> animations_;
	std::string                                currentAnim_;
	int                                        currentFrame_ = 0;
	float                                      timer_        = 0.0f;
};

#endif // ANIMATED_SPRITE_HPP
