#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <functional>

class Renderer;

struct Drawable {
	float                          sortY;
	std::function<void(Renderer&)> draw;
};

#endif // DRAWABLE_HPP
