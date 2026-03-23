#ifndef COLLISION_RESOLVER_HPP
#define COLLISION_RESOLVER_HPP

class Map;
class Entity;

class CollisionResolver {
public:
	CollisionResolver(Entity& entity, Map& map);

	void resolve();

private:
	Entity& entity_;
	Map&    map_;
};

#endif // COLLISION_RESOLVER_HPP
