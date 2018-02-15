#ifndef SDL2OPENGL_CAMERA_H
#define SDL2OPENGL_CAMERA_H

#include "Observer.h"
#include "Vector3d.h"
#include "EventDispatcher.h"
#include "GameData.h"
#include "SmoothTranslation.h"

enum Direction {
	Forward, Backward, Up, Down, Left, Right
};

enum CamMode {
	Build, Orbit, Tank, FirstPerson, God, Fly
};

/// \brief Classe représentant une caméra dans un environnement 3D

class Camera : public Observer<SDL_Event*>, public Singleton<Camera> {
private:
	CamMode mode;	///< Mode de la caméra
	double cap;	///< Angle de vue maximal de la caméra
	double moveDistance;	///< Distance de mouvement de la caméra
	Vector3d up;	///< Vecteur représentant l'inclinaison de la caméra
	bool mousePressed;	///< Booléen retournant vrai si la souris est appuyée
	SmoothTranslation<1> zoomSpeed;	///< Distance zoomée à chaque tick
	SmoothTranslation<1> verticalSpeed;	///< Distance zoomée à chaque tick
	SmoothTranslation<2> lookSpeed;	///< Distance de rotation à chaque tick
	const unsigned char zoomMin;	///< Zoom minimal
	const unsigned char zoomMax;	///< Zoom maximal
	bool yAligned;	///< Si vrai, shiftFrustum envoi un déplacement perpendiculaire au plan formé par l'axe des x et z

public:
	Vector3d pos;	///< Vecteur représentant la position de la caméra dans l'environnement 3D
	Vector3d target;	///< Vecteur représentant la position de la cible dans l'environnement 3D

	Camera() : zoomSpeed(0.3, 0.8, 0.01, 1.5),
			   zoomMin(10),
			   zoomMax(50),
			   lookSpeed(0.0005, 0.9, 0.001, 50.),
			   verticalSpeed(0.5, 0.8, 0.01, 1.5) {}

	/// \brief Détermine tous les aspects de la caméra
	/// \param mode Mode de la caméra
	/// \param moveDistance Distance de mouvement de la caméra
	/// \param sensitivity Sensibilité de la caméra
	/// \param cap Angle de vue maximal (Distance par rapport à l'axe des y en x)
	/// \param pos Vecteur représentant la position de la cible dans l'environnement 3D
	/// \param target Vecteur représentant la position de la cible dans l'environnement 3D
	/// \param up Vecteur représentant l'inclinaison de la caméra
	void setCamera(CamMode mode, double moveDistance, double cap, Vector3d pos, Vector3d target, Vector3d up, bool yAligned) {
		this->yAligned = yAligned;
		this->mode = mode;
		this->moveDistance = moveDistance;
		this->cap = cap;
		this->pos = pos;
		this->target = target;
		this->up = up;
		this->mousePressed = false;

		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONUP, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEWHEEL, this);
		EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
	}

	~Camera() {
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEWHEEL, this);
		EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);
	}

	/// \brief Permet de mettre à jour la caméra à chaque tick
	void update() {
		zoomSpeed.update();
		lookSpeed.update();
		verticalSpeed.update();

		zoom(zoomSpeed[0]);
		look(lookSpeed[0], lookSpeed[1]);
		pos.y += verticalSpeed[0];
		target.y += verticalSpeed[0];
	}

	/// \brief Permet de définir la matrice de vue du SDLGLContext
	void setView() {
		SDLGLContext::setView(pos, target, up);
	}

	/// \brief Permet de regarder selon un mouvement relatif de la souris
	/// \param xrel Mouvement relatif en x
	/// \param yrel Mouvement relatif en y
	void look(double xrel, double yrel) {
		double length;
		Vector3d dir = (target - pos).normalize();
		Vector3d oldDirFlat(dir.x, 0.0, dir.z);
		Vector3d xAxis;

		if (mode == Orbit) {
			Math::rotateAroundLine(-xrel, Vector3d::UP, target, pos);

			xAxis = Vector3d::UP / dir;

			Math::rotateAroundLine(yrel, xAxis, target, pos);

			dir = (target - pos).normalize();

			if ((length = std::sqrt(dir.x * dir.x + dir.z * dir.z)) <= cap) {
				(dir *= (cap / length)).y = (dir.y > 0) ? std::sqrt(1 - cap * cap) : -std::sqrt(1 - cap * cap);;
				pos = target - (target - pos).getNorm() * dir;
			}
		}else {
			Math::rotateAroundLine(-xrel, up, pos, target);

			xAxis = up / dir;

			Math::rotateAroundLine(yrel, xAxis, pos, target);

			if (mode == Fly)
				up = (dir / xAxis).normalize();
			else {
				dir = (target - pos).normalize();

				if ((length = std::sqrt(dir.x * dir.x + dir.z * dir.z)) <= cap) {
					(dir *= (cap / length)).y = (dir.y > 0) ? std::sqrt(1 - cap * cap) : -std::sqrt(1 - cap * cap);;
					target = pos + dir;
				}
			}
		}
	}

	/// \brief "Do a barrel roll"
	/// \param positive Booléen représentant le sens de la rotation, vrai représente le sens horaire, faux représente le sens anti-horaire
	void roll(bool positive) {
		if (mode == Fly) {
			Vector3d dir = target - pos;

			Math::rotateAroundLine(positive ? 0.05 : -0.05, dir, Vector3d::ZERO, up);
		}
	}

	/// \brief Permet de faire bouger la caméra selon son mode
	/// \param direction Direction du mouvement de la caméra
	void move(Direction direction) {
		switch (direction) {
			case Forward:
				if (mode >= FirstPerson) {
					Vector3d dir = target - pos;
					dir.normalize() *= moveDistance;
					if (mode >= God) {
						pos.y += dir.y;
						target.y += dir.y;
					}else {
						dir.y = 0.0;
					}

					pos.x += dir.x;
					target.x += dir.x;

					pos.z += dir.z;
					target.z += dir.z;
				}else {
					target.z -= moveDistance;
					pos.z -= moveDistance;
				}
				break;
			case Backward:
				if (mode >= FirstPerson) {
					Vector3d dir = pos - target;

					if (mode >= God) {
						dir.normalize() *= moveDistance;

						pos.y += dir.y;
						target.y += dir.y;
					}else {
						dir.y = 0.0;
						dir.normalize() *= moveDistance;
					}

					pos.x += dir.x;
					target.x += dir.x;

					pos.z += dir.z;
					target.z += dir.z;
				}else {
					target.z += moveDistance;
					pos.z += moveDistance;
				}
				break;
			case Up:
				target.y += moveDistance * up.y;
				pos.y += moveDistance * up.y;
				break;
			case Down:
				target.y -= moveDistance * up.y;
				pos.y -= moveDistance * up.y;
				break;
			case Left:
				if (mode >= FirstPerson) {
					Vector3d dir = (up / (target - pos)).normalize() *= moveDistance;
					pos.x += dir.x;
					target.x += dir.x;

					if (mode >= God) {
						pos.y += dir.y;
						target.y += dir.y;
					}

					pos.z += dir.z;
					target.z += dir.z;
				}else {
					target.x -= moveDistance;
					pos.x -= moveDistance;
				}
				break;
			case Right:
				if (mode >= FirstPerson) {
					Vector3d dir = (up / (target - pos)).normalize() *= -moveDistance;
					pos.x += dir.x;
					target.x += dir.x;

					if (mode >= God) {
						pos.y += dir.y;
						target.y += dir.y;
					}

					pos.z += dir.z;
					target.z += dir.z;
				}else {
					target.x += moveDistance;
					pos.x += moveDistance;
				}
		}
	}

	///\brief Permet de faire zoomer la caméra sur son target
	///\param zoom Déplacement zoom-in ou zoom-out
	void zoom(double zoom) {
		pos += (target - pos).normalize() * zoom;
	}

	/// \brief Change le mode de la caméra
	/// \param mode Mode qui sera attribuer
	void setMode(CamMode mode) {
		this->mode = mode;
		if (mode != Fly)
			up.set(0.0, 1.0, 0.0);
	}

	/// \brief Retourne un vecteur de position sur le plan éloigné de d équivalent à la position en pixels
	/// \param x Composante x de la position en pixel
	/// \param y Composante y de la position en pixel
	/// \param d Distance du plan éloigné de la caméra
	/// \return Le vecteur de position
	Vector3d screenToFrustumPlane(double x, double y, double d) {
		Vector3d front = (target - pos).normalize();
		Vector3d rightVec = (Vector3d::UP / front).normalize();
		Vector3d newUp = (front / rightVec).normalize();

		return pos + d * (front + TAN_THETA_OVER_TWO * (rightVec * (1 - (2 * x) / (double)WINDOW_WIDTH) + newUp * ((double)WINDOW_HEIGHT - 2 * y) / (double)WINDOW_WIDTH));
	}

	/// \brief Retourne un vecteur de déplacement sur le plan éloigné de d équivalent au déplacement en pixels
	/// \param dx Composante x du déplacement en pixel
	/// \param dy Composante y du déplacement en pixel
	/// \param d Distance du plan éloigné de la caméra
	/// \return Le vecteur de déplacement
	Vector3d shiftScreenToFrustumPlane(double dx, double dy, double d) {
		Vector3d front = (target - pos).normalize();
		Vector3d rightVec = (Vector3d::UP / front).normalize();

		Vector3d newUp;
		yAligned ? newUp = Vector3d::UP : newUp = (front / rightVec).normalize();

		return ((-2 * d * TAN_THETA_OVER_TWO) / (double)WINDOW_WIDTH) * (rightVec * dx + newUp * dy);
	}

	/// \brief Notification de l'action en cours
	/// \param event Structure contenant l'information sur l'action de l'utilisateur
	void notify(SDL_Event* event) {
		switch (event->type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event->button.button == SDL_BUTTON_RIGHT)
					mousePressed = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mousePressed = false;
				break;
			case SDL_MOUSEMOTION:
				if (mousePressed)
					lookSpeed.accelerate({(double)event->motion.xrel, (double)event->motion.yrel});
				break;
			case SDL_MOUSEWHEEL:
				if ((mode == Orbit) || (mode == Build)) {
					double zoom = (target - pos).getNorm();
					if (event->wheel.y > 0) {
						if (zoom > zoomMin)
							zoomSpeed.accelerate({(double)event->wheel.y});
					}else if (zoom < zoomMax)
						zoomSpeed.accelerate({(double)event->wheel.y});
				}
				break;
			case SDL_KEYDOWN:
				if ((mode != Orbit) && (mode != Build)) {
					switch (event->key.keysym.sym) {
						case SDLK_w:
							move(Forward);
							break;
						case SDLK_s:
							move(Backward);
							break;
						case SDLK_a:
							move(Left);
							break;
						case SDLK_d:
							move(Right);
							break;
						case SDLK_SPACE:
							move(Up);
							break;
						case SDLK_LSHIFT:
							move(Down);
						default:;
					}
				}

				if (mode == Orbit || mode == Build) {
					/*if (event->key.keysym.sym == SDLK_w)
						verticalSpeed.accelerate({up.y});
					else if (event->key.keysym.sym == SDLK_s)
						verticalSpeed.accelerate({-up.y});*/

					if (event->key.keysym.sym == SDLK_KP_4)
						Math::rotateAroundLine(90, Vector3d(0, 1, 0), target, pos);
					else if (event->key.keysym.sym == SDLK_KP_6)
						Math::rotateAroundLine(-90, Vector3d(0, 1, 0), target, pos);
				}
			default:;
		}
	}
};

#endif //SDL2OPENGL_CAMERA_H
