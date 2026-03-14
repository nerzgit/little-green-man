#include "LightReceiver.hpp"

LightReceiver::LightReceiver() : activated_(false) {
}

LightReceiver::LightReceiver(glm::vec2 pos, int tileId)
    : LightTile(pos, tileId), activated_(false) {
}

LightReceiver::~LightReceiver() {
}

void LightReceiver::setActivated(bool activated) {
	activated_ = activated;
}

bool LightReceiver::isActivated() const {
	return activated_;
}
