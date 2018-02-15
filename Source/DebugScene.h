#ifndef SOURCE_DEBUGSCENE_H
#define SOURCE_DEBUGSCENE_H

#include <iostream>
#include "Scene.h"
#include "AABB.h"
#include "Line.h"

class DebugScene : public Scene {
private:
	AABB box;
	Line line;
	bool moveA = true;
	bool moveB = true;
	bool intersects = false;

protected:

	/// \brief Permet l'affichage à l'écran
	/// \param glContext SDLGLContext à qui s'occupera de l'affichage
	virtual void draw3d() {
		Scene::draw3d();

		box.draw(intersects ? Vector3d::UP : Vector3d::RIGHT);
		line.draw();
	}

public:
	DebugScene() : Scene(),
			//sphere(.5, SDLGLContext::loadTexture("Textures3D/Piece/Skins/Dipyramid"), true),
			//planet(1., "IRON", Vector3d::FRONT, Vector3d::ZERO),
				   box(-.5 * Vector3d::ONE, .5 * Vector3d::ONE),
				   line(4. * Vector3d::UP, 3. * Vector3d::UP, Vector3d::UP) {
		EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
	}

	~DebugScene() {
		EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);
	}

	void notify(SDL_Event* event) {
		switch (event->key.keysym.sym) {
			case SDLK_SPACE:
				if (moveA)
					line.a.y += .1;
				if (moveB)
					line.b.y += .1;
				break;
			case SDLK_r:
				if (moveA)
					line.a.y -= .1;
				if (moveB)
					line.b.y -= .1;
				break;
			case SDLK_s:
				if (moveA)
					line.a.z += .1;
				if (moveB)
					line.b.z += .1;
				break;
			case SDLK_w:
				if (moveA)
					line.a.z -= .1;
				if (moveB)
					line.b.z -= .1;
				break;
			case SDLK_d:
				if (moveA)
					line.a.x += .1;
				if (moveB)
					line.b.x += .1;
				break;
			case SDLK_a:
				if (moveA)
					line.a.x -= .1;
				if (moveB)
					line.b.x -= .1;
				break;
			case SDLK_j:
				moveA = true;
				moveB = false;
				break;
			case SDLK_k:
				moveA = moveB = true;
				break;
			case SDLK_l:
				moveA = false;
				moveB = true;
				break;
		}

		//Intersect intersect(planet.isColliding(false, line.b - line.a, line.a));
		//std::cout << "Collided:\t" << std::boolalpha << intersect.collided << "\npos:\t" << intersect.pos.toString() << "\nr:\t" << intersect.r << "\n" << std::endl;
		std::cout << "Position\t" << line.a.toString() << "\nDirection:\t" << (line.b - line.a).toString() << "\n"
				  << std::boolalpha << (intersects = box.intersects(false, line.b - line.a, line.a)) << std::endl;
	}
};

#endif //SOURCE_DEBUGSCENE_H
