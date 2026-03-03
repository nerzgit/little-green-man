#ifndef MAP1_1_HPP
#define MAP1_1_HPP

#include "../Map.hpp"
#include "../wall/GrayBlock.hpp"

class Map1_1 : public Map {
public:
	Map1_1();
	~Map1_1();

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onDraw(Renderer& renderer) override;
	void onEnd() override;

private:
	GrayBlock wallBlock_;
};

#endif // MAP1_1_HPP
