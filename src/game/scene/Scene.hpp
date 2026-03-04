#ifndef SCENE_HPP
#define SCENE_HPP

class Renderer;
class SceneManager;

class Scene {
public:
	virtual ~Scene() = default;

	virtual void update(float deltaTime, SceneManager& sm) = 0;
	virtual void render(Renderer& renderer)                = 0;
};

#endif // SCENE_HPP
