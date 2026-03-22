#ifndef GAME_CONSTANTS_HPP
#define GAME_CONSTANTS_HPP

namespace GameConstants {

inline constexpr const char* kGameTitle = "Little Green Man";

// ワールド座標でのタイル1枚のサイズ（ピクセル）
inline constexpr float kTileSize = 64.0f;

// タイルセット画像での1タイルのピクセルサイズ
inline constexpr int kTilePixels = 32;

} // namespace GameConstants

#endif // GAME_CONSTANTS_HPP
