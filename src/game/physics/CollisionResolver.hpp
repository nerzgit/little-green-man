#ifndef COLLISION_RESOLVER_HPP
#define COLLISION_RESOLVER_HPP

class Entity;
class Map;

class CollisionResolver {
public:
	CollisionResolver(Entity& entity, Map& map, float stageWidth,
	                  float stageHeight);

	void resolve();

private:
	Entity& entity_;
	Map&    map_;
	float   stageWidth_;
	float   stageHeight_;
};

#endif // COLLISION_RESOLVER_HPP
