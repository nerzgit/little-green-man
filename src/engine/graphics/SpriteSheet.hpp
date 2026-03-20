#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include "AnimatedSprite.hpp"
#include "Texture.hpp"
#include <memory>

/**
 * 単一テクスチャのスプライトシートから、col/row 指定で SpriteFrame を切り出すクラス。
 * フレームサイズ（ピクセル）はコンストラクタで指定する。
 */
class SpriteSheet {
public:
	SpriteSheet(std::shared_ptr<Texture> texture, int frameWidth,
	            int frameHeight);

	/** col 列・row 行のフレームを SpriteFrame として返す */
	SpriteFrame frame(int col, int row) const;

private:
	std::shared_ptr<Texture> texture_;
	int                      frameWidth_;
	int                      frameHeight_;
};

#endif // SPRITE_SHEET_HPP
