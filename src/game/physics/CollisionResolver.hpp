#ifndef COLLISION_RESOLVER_HPP
#define COLLISION_RESOLVER_HPP

class Player;
class Map;

class CollisionResolver {
public:
	CollisionResolver(Player& player, Map& map, float stageWidth,
	                  float stageHeight);

	void resolve();

private:
	Player& player_;
	Map&    map_;
	float   stageWidth_;
	float   stageHeight_;
};

#endif // COLLISION_RESOLVER_HPP
