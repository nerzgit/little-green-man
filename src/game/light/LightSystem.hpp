#ifndef LIGHT_SYSTEM_HPP
#define LIGHT_SYSTEM_HPP

#include "../../engine/graphics/Texture.hpp"
#include "../IDrawableSource.hpp"
#include "LightBeam.hpp"
#include "LightMirror.hpp"
#include "LightReceiver.hpp"
#include "LightSource.hpp"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class Renderer;
class Map;

class LightSystem : public IDrawableSource {
public:
	LightSystem();
	~LightSystem();

	void update(float deltaTime, Map& map);
	void collectDrawables(std::vector<Drawable>& out) const override;
	void draw(Renderer& renderer) const; // ビームのみ描画
	void trace(Map& map);
	bool isReceiverActivated() const;
	int  getActivatedReceiverCount() const;

	bool isMirrorAt(const glm::vec2& position) const;
	bool tryPush(const glm::vec2& touchPos, glm::vec2 pushDir, Map& map);

	// レシーバー数に応じて変わる全画面オーバーレイ（最大8段階）
	void setOverlayColor(int index, const glm::vec4& rgba);
	void drawOverlay(Renderer& renderer, int windowWidth, int windowHeight) const;

	void setBeamThickness(float thickness) {
		lightBeam_.thickness = thickness;
	}

	void loadTileset(const std::string& path);
	void loadLightSources(const std::string& path);
	void loadMirrors(const std::string& path);
	void loadReceivers(const std::string& path);

private:
	static constexpr int kMaxSteps = 100;

	std::shared_ptr<Texture> tileset_;
	int                      tilesetCols_ = 0;

	std::vector<LightSource>   lightSources_;
	std::vector<LightMirror>   lightMirrors_;
	std::vector<LightReceiver> lightReceivers_;
	LightBeam                  lightBeam_;

	static constexpr int                  kMaxOverlaySteps = 8;
	std::array<glm::vec4, kMaxOverlaySteps> overlayColors_ = {};

	void           drawTile(Renderer& renderer, int tileId,
	                        const glm::vec2& position) const;
	LightMirror*   getMirrorAt(const glm::vec2& position);
	LightReceiver* getReceiverAt(const glm::vec2& position);
};

#endif // LIGHT_SYSTEM_HPP