#ifndef MAP1_2_HPP
#define MAP1_2_HPP

#include "../Map.hpp"
#include "../wall/GrayBlock.hpp"

class Map1_2 : public Map {
public:
	Map1_2();
	~Map1_2();

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onDraw(Renderer& renderer) override;
	void onEnd() override;

private:
	GrayBlock wallBlock_;
};

#endif // MAP1_2_HPP
