#ifndef LIGHT_BEAM_HPP
#define LIGHT_BEAM_HPP

#include "Direction.hpp"
#include <glm/glm.hpp>
#include <vector>

class Renderer;

// 光源・鏡・終点などのキーポイント
struct BeamWaypoint {
	glm::vec2 position;
	Direction
	  outDirection; // このポイントから出ていく方向（終点では使用しない）
};

class LightBeam {
public:
	float thickness = 10.0f;

	LightBeam();
	~LightBeam();
	void clear();
	void draw(Renderer& renderer) const;
	void addWaypoint(const glm::vec2& position, Direction outDir);

private:
	std::vector<BeamWaypoint> waypoints_;
};

#endif // LIGHT_BEAM_HPP