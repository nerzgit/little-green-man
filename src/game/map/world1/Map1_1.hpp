#ifndef MAP1_1_HPP
#define MAP1_1_HPP

#include "../Map.hpp"

class Map1_1 : public Map {
public:
	Map1_1();
	~Map1_1();

	MapEvent             update(float deltaTime, Player& player) override;
	std::unique_ptr<Map> createNextMap() override;
	std::string          getStoryPath() const override { return "assets/story/map1_1.json"; }
};

#endif // MAP1_1_HPP
