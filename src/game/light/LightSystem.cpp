#include "LightSystem.hpp"
#include "../../engine/graphics/Renderer.hpp"
#include "../../engine/graphics/Texture.hpp"
#include "../map/Map.hpp"
#include "../map/MapLoader.hpp"
#include "LightMirror.hpp"
#include "LightSource.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <sstream>
#include <stdexcept>

LightSystem::LightSystem() {
}

LightSystem::~LightSystem() {
}

void LightSystem::update(float deltaTime, Map& map) {
	for (auto& mirror : lightMirrors_)
		mirror.update(deltaTime);
}

void LightSystem::drawTile(Renderer& renderer, int tileId,
                           const glm::vec2& position) const {
	if (!tileset_ || tileId < 0)
		return;
	const float ts      = MapLoader::kTileSize;
	const float srcSize = static_cast<float>(kTilePixels);
	const float texW    = static_cast<float>(tileset_->width);
	const float texH    = static_cast<float>(tileset_->height);
	int         tileCol = tileId % tilesetCols_;
	int         tileRow = tileId / tilesetCols_;
	glm::vec2 uvMin  = {(tileCol * srcSize) / texW, (tileRow * srcSize) / texH};
	glm::vec2 uvMax  = {((tileCol + 1) * srcSize) / texW,
	                    ((tileRow + 1) * srcSize) / texH};
	glm::vec2 center = {position.x + ts / 2.0f, position.y + ts / 2.0f};
	renderer.drawTile(*tileset_, center, ts, ts, uvMin, uvMax);
}

void LightSystem::draw(Renderer& renderer) const {
	for (const auto& mirror : lightMirrors_) {
		drawTile(renderer, mirror.getTileId(), mirror.getDrawPosition());
	}
	for (const auto& receiver : lightReceivers_) {
		drawTile(renderer, receiver.getTileId(), receiver.position);
	}
	for (const auto& source : lightSources_) {
		drawTile(renderer, source.getTileId(), source.position);
	}
	lightBeam_.draw(renderer);
}

void LightSystem::trace(Map& map) {
	lightBeam_.clear();

	for (const auto& source : lightSources_) {
		glm::vec2 lightPos = source.position;
		Direction dir      = source.getDirection();

		// 光源を最初のウェイポイントとして登録
		lightBeam_.addWaypoint(lightPos, dir);

		for (int step = 0; step < kMaxSteps; ++step) {
			glm::vec2 nextPos =
			  nextPosition(lightPos, dir, MapLoader::kTileSize);

			// 壁: 壁の位置を終点として登録して停止
			if (map.isWallAt(nextPos.x, nextPos.y)) {
				lightBeam_.addWaypoint(nextPos, dir);
				break;
			}

			// ミラー: 入ってきた方向で終点登録→反射後の方向で始点登録して継続
			if (LightMirror* mirror = getMirrorAt(nextPos)) {
				dir      = mirror->redirect(dir);
				lightPos = nextPos;
				lightBeam_.addWaypoint(lightPos, dir);
				continue;
			}

			// 受信機: 受信機の位置を終点として登録してアクティブにして停止
			if (LightReceiver* receiver = getReceiverAt(nextPos)) {
				receiver->setActivated(true);
				lightBeam_.addWaypoint(nextPos, dir);
				break;
			}

			// ステージ外: ステージ端を終点として登録して停止
			if (nextPos.x < 0 || nextPos.y < 0
			    || nextPos.x >= map.getPixelWidth()
			    || nextPos.y >= map.getPixelHeight()) {
				lightBeam_.addWaypoint(nextPos, dir);
				break;
			}

			// 何もない: 次のタイルへ進む
			lightPos = nextPos;
		}
	}
}

bool LightSystem::isReceiverActivated() const {
	for (const auto& receiver : lightReceivers_) {
		if (receiver.isActivated()) {
			return true;
		}
	}
	return false;
}

int LightSystem::getActivatedReceiverCount() const {
	int count = 0;
	for (const auto& receiver : lightReceivers_) {
		if (receiver.isActivated())
			++count;
	}
	return count;
}

void LightSystem::setOverlayColor(int index, const glm::vec4& rgba) {
	if (index >= 0 && index < kMaxOverlaySteps)
		overlayColors_[index] = rgba;
}

void LightSystem::drawOverlay(Renderer& renderer, int windowWidth,
                              int windowHeight) const {
	int          activated = getActivatedReceiverCount();
	int          idx       = std::min(activated, kMaxOverlaySteps - 1);
	const auto&  c         = overlayColors_[idx];
	glm::vec2    center = {windowWidth / 2.0f, windowHeight / 2.0f};
	renderer.drawRectHUD(center, static_cast<float>(windowWidth),
	                     static_cast<float>(windowHeight),
	                     {c.r, c.g, c.b}, c.a);
}

LightMirror* LightSystem::getMirrorAt(const glm::vec2& position) {
	const float ts  = MapLoader::kTileSize;
	int         col = static_cast<int>(position.x / ts);
	int         row = static_cast<int>(position.y / ts);
	for (auto& mirror : lightMirrors_) {
		int mCol = static_cast<int>(mirror.position.x / ts);
		int mRow = static_cast<int>(mirror.position.y / ts);
		if (mCol == col && mRow == row) {
			return &mirror;
		}
	}
	return nullptr;
}

LightReceiver* LightSystem::getReceiverAt(const glm::vec2& position) {
	const float ts  = MapLoader::kTileSize;
	int         col = static_cast<int>(position.x / ts);
	int         row = static_cast<int>(position.y / ts);
	for (auto& receiver : lightReceivers_) {
		int rCol = static_cast<int>(receiver.position.x / ts);
		int rRow = static_cast<int>(receiver.position.y / ts);
		if (rCol == col && rRow == row) {
			return &receiver;
		}
	}
	return nullptr;
}

bool LightSystem::isMirrorAt(const glm::vec2& position) const {
	const float ts  = MapLoader::kTileSize;
	int         col = static_cast<int>(position.x / ts);
	int         row = static_cast<int>(position.y / ts);
	for (const auto& mirror : lightMirrors_) {
		// 実際の位置で判定
		int mCol = static_cast<int>(mirror.position.x / ts);
		int mRow = static_cast<int>(mirror.position.y / ts);
		if (mCol == col && mRow == row)
			return true;
		// アニメーション中は描画位置でも判定（プレイヤーをブロック）
		if (mirror.isMoving()) {
			glm::vec2 drawPos = mirror.getDrawPosition();
			int       dCol    = static_cast<int>(drawPos.x / ts);
			int       dRow    = static_cast<int>(drawPos.y / ts);
			if (dCol == col && dRow == row)
				return true;
		}
	}
	return false;
}

bool LightSystem::tryPush(const glm::vec2& touchPos, glm::vec2 pushDir,
                          Map& map) {
	LightMirror* mirror = getMirrorAt(touchPos);
	if (!mirror)
		return false;

	// アニメーション中は押せない（プレイヤーを壁扱いでブロック）
	if (mirror->isMoving())
		return false;

	const float ts     = MapLoader::kTileSize;
	glm::vec2   newPos = mirror->position + pushDir * ts;

	// ステージ外チェック
	if (newPos.x < 0 || newPos.y < 0 || newPos.x + ts > map.getPixelWidth()
	    || newPos.y + ts > map.getPixelHeight()) {
		return false;
	}

	// 壁チェック（移動先タイル中央で判定）
	glm::vec2 newCenter = newPos + glm::vec2(ts / 2.0f, ts / 2.0f);
	if (map.isWallAt(newCenter.x, newCenter.y))
		return false;

	// 別のミラーが移動先にいないかチェック
	if (getMirrorAt(newPos))
		return false;

	glm::vec2 oldPos = mirror->position;
	mirror->position = newPos;
	mirror->startMove(oldPos);
	return true;
}

void LightSystem::loadTileset(const std::string& path) {
	tileset_     = std::make_unique<Texture>(path);
	tilesetCols_ = tileset_->width / kTilePixels;
}

// CSV を走査し、tileId != -1 のセルごとに onTile(tileId, pos)
// を呼ぶ共通ヘルパー
static void
parseTileCsv(const std::string&                                path,
             const std::function<void(int, const glm::vec2&)>& onTile) {
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open CSV: " + path);
	}
	std::string line;
	int         row = 0;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string       cell;
		int               col = 0;
		while (std::getline(ss, cell, ',')) {
			int tileId = std::stoi(cell);
			if (tileId != -1) {
				glm::vec2 pos(col * MapLoader::kTileSize,
				              row * MapLoader::kTileSize);
				onTile(tileId, pos);
			}
			++col;
		}
		++row;
	}
}

void LightSystem::loadLightSources(const std::string& path) {
	lightSources_.clear();
	parseTileCsv(path, [&](int tileId, const glm::vec2& pos) {
		auto it = kSourceTileIdToDirection.find(tileId);
		if (it != kSourceTileIdToDirection.end()) {
			lightSources_.emplace_back(pos, it->second, tileId);
		}
	});
}

void LightSystem::loadMirrors(const std::string& path) {
	lightMirrors_.clear();
	parseTileCsv(path, [&](int tileId, const glm::vec2& pos) {
		auto it = kMirrorTileIdToDirection.find(tileId);
		if (it != kMirrorTileIdToDirection.end()) {
			lightMirrors_.emplace_back(pos, it->second, tileId);
		}
	});
}

void LightSystem::loadReceivers(const std::string& path) {
	lightReceivers_.clear();
	parseTileCsv(path, [&](int tileId, const glm::vec2& pos) {
		lightReceivers_.emplace_back(pos, tileId);
	});
}
