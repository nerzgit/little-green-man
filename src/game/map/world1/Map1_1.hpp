#ifndef MAP1_1_HPP
#define MAP1_1_HPP

#include "../Map.hpp"

class Map1_1 : public Map {
public:
	Map1_1();
	~Map1_1();

	std::unique_ptr<Map> createNextMap() override;
	std::string          getStoryPath() const override {
        return "assets/maps/1_1_story.json";
	}
};

#endif // MAP1_1_HPP
