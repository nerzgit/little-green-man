#ifndef LIGHT_RECEIVER_HPP
#define LIGHT_RECEIVER_HPP

#include "LightTile.hpp"

class LightReceiver : public LightTile {
public:
	LightReceiver();
	LightReceiver(glm::vec2 pos, int tileId);
	~LightReceiver();

	void setActivated(bool activated);
	bool isActivated() const;

private:
	bool activated_;
};

#endif // LIGHT_RECEIVER_HPP
