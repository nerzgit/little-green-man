#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>

class Entity {
public:
	glm::vec2 position;
	glm::vec2 velocity;
	float     size;
	bool      active;

	Entity() : position(0, 0), velocity(0, 0), size(10.0f), active(true) {
	}
	Entity(glm::vec2 pos, float size)
	    : position(pos), velocity(0, 0), size(size), active(true) {
	}

	virtual ~Entity() = default;

	virtual void update(float deltaTime) {
		if (active) {
			position += velocity * deltaTime;
		}
	}

	bool intersects(const Entity& other) const {
		float distance = glm::distance(position, other.position);
		return distance < (size + other.size);
	}
};

#endif // ENTITY_HPP
