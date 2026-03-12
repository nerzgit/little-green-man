#include "LightReceiver.hpp"

LightReceiver::LightReceiver() : activated_(false), tileId_(-1) {
}

LightReceiver::LightReceiver(glm::vec2 pos, int tileId)
    : Entity(pos, 10.0f), activated_(false), tileId_(tileId) {
}

LightReceiver::~LightReceiver() {
}

void LightReceiver::setActivated(bool activated) {
	activated_ = activated;
}

bool LightReceiver::isActivated() const {
	return activated_;
}
