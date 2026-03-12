#ifndef LIGHT_RECEIVER_HPP
#define LIGHT_RECEIVER_HPP

#include "../../engine/core/Entity.hpp"

class LightReceiver : public Entity {
public:
	LightReceiver();
	LightReceiver(glm::vec2 pos, int tileId);
	~LightReceiver();

	void setActivated(bool activated);
	bool isActivated() const;
	int  getTileId() const { return tileId_; }

private:
	bool activated_;
	int  tileId_;
};

#endif // LIGHT_RECEIVER_HPP
