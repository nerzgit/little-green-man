#include "YSortRenderer.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../Drawable.hpp"
#include "../World.hpp"
#include "../map/Map.hpp"
#include "../player/Player.hpp"
#include <algorithm>
#include <vector>

void YSortRenderer::draw(World& world, Renderer& renderer) {
	std::vector<Drawable> drawables;
	world.map().collectDrawables(drawables);
	if (world.player().active)
		drawables.push_back(
		  {world.player().position.y,
		   [&](Renderer& r) { world.player().draw(r); }});
	std::sort(
	  drawables.begin(), drawables.end(),
	  [](const Drawable& a, const Drawable& b) { return a.sortY < b.sortY; });
	for (auto& d : drawables)
		d.draw(renderer);
}
