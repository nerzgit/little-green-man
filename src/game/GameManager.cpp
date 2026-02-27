#include "GameManager.hpp"
#include <GLFW/glfw3.h>

// 16x16 pixel art: little green man
// Row 0 = top (antennae tips), Row 15 = bottom (feet)
// .=transparent  G=green  D=dark green  W=white  B=black  A=gray  Y=yellow
static const char* kSpriteMap[16] = {
    "......Y..Y......",  // 0  antenna tips
    "......A..A......",  // 1  antennae
    ".....A....A.....",  // 2  antennae spreading
    "....GGGGGGGG....",  // 3  head
    "...GGGGGGGGGG...",  // 4  head
    "...GGWBGGWBGG...",  // 5  eyes (W=white, B=pupil)
    "...GGWWGGWWGG...",  // 6  eyes lower
    "...GGGGGGGGGG...",  // 7  head
    "...GGGDDDDGGG...",  // 8  mouth
    "....GGGGGGGG....",  // 9  neck
    "..GGGGGGGGGGGG..",  // 10 body
    ".GGGGGGGGGGGGGG.",  // 11 body wide
    ".GGGGGGGGGGGGGG.",  // 12 body wide
    "..GGGGGGGGGGGG..",  // 13 body
    "....GGG..GGG....",  // 14 legs
    "....GGG..GGG....",  // 15 legs
};

static std::unique_ptr<Texture> createPlayerTexture() {
	constexpr int W = 16;
	constexpr int H = 16;

	// RGBA palette
	constexpr unsigned char T[] = {0,   0,   0,   0};    // transparent
	constexpr unsigned char G[] = {80,  200, 100, 255};  // green
	constexpr unsigned char D[] = {40,  120, 60,  255};  // dark green
	constexpr unsigned char Wh[] = {255, 255, 255, 255}; // white
	constexpr unsigned char B[] = {30,  30,  30,  255};  // black
	constexpr unsigned char A[] = {160, 160, 160, 255};  // gray antenna
	constexpr unsigned char Y[] = {240, 200, 0,   255};  // yellow tip

	unsigned char pixels[W * H * 4];

	for (int row = 0; row < H; row++) {
		for (int col = 0; col < W; col++) {
			int                  idx = (row * W + col) * 4;
			char                 c   = kSpriteMap[row][col];
			const unsigned char* px;
			switch (c) {
				case 'G': px = G;  break;
				case 'D': px = D;  break;
				case 'W': px = Wh; break;
				case 'B': px = B;  break;
				case 'A': px = A;  break;
				case 'Y': px = Y;  break;
				default:  px = T;  break;
			}
			pixels[idx]     = px[0];
			pixels[idx + 1] = px[1];
			pixels[idx + 2] = px[2];
			pixels[idx + 3] = px[3];
		}
	}

	return std::make_unique<Texture>(W, H, pixels);
}

GameManager::GameManager(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	player_ = std::make_unique<Player>(
	  glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f));

	playerTexture_ = createPlayerTexture();
}

void GameManager::update(float deltaTime) {
	updatePlayer(deltaTime);
}

void GameManager::render(Renderer& renderer) {
	renderer.clear();

	if (player_->active) {
		constexpr float kSpriteSize = 64.0f;
		renderer.drawSprite(*playerTexture_, player_->position, kSpriteSize,
		                    kSpriteSize);
	}
}

void GameManager::updatePlayer(float deltaTime) {
	player_->update(deltaTime);

	if (player_->position.x < player_->size) {
		player_->position.x = player_->size;
	}
	if (player_->position.x > windowWidth_ - player_->size) {
		player_->position.x = windowWidth_ - player_->size;
	}
	if (player_->position.y < player_->size) {
		player_->position.y = player_->size;
	}
	if (player_->position.y > windowHeight_ - player_->size) {
		player_->position.y = windowHeight_ - player_->size;
	}
}
