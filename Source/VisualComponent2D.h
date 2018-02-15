/// \brief Base de tous composants visuel en 2D.
///
/// Permet la gestion des textures et des événements en relation au compsant visuel.
///

#ifndef SOURCE_VISUALCOMPONENT2D_H
#define SOURCE_VISUALCOMPONENT2D_H

#include <functional>
#include <SDL2/SDL.h>
#include "Observer.h"
#include "EventDispatcher.h"
#include "Position.h"

class VisualComponent2D : public Observer<SDL_Event*> {
protected:
	bool mouseIsOver;    ///< Vrai si le pointeur de souris survol le composant.

	/// \brief Dessine un rectangle sur la région du VisualComponent2D.
	/// \param textured Vrai si la texture doit être utilisée
	void drawQuad(const bool& textured) {
		glPushMatrix();
		if (angle) {
			glTranslated(pos.getGlobalX() + pivot.first, pos.getGlobalY() + pivot.second, 0.);
			glRotated(Math::radToDeg(angle), 0., 0., -1.);
			glTranslated(-pivot.first, -pivot.second, 0.);
		}else
			glTranslated(pos.getGlobalX(), pos.getGlobalY(), 0.);

		if (textured) {
			glBindTexture(GL_TEXTURE_2D, texIds[currTexIndex]);

			glBegin(GL_QUADS);
			glTexCoord2i(0, 0);
			glVertex2i(0, 0);
			glTexCoord2i(1, 0);
			glVertex2i(w, 0);
			glTexCoord2i(1, 1);
			glVertex2i(w, h);
			glTexCoord2i(0, 1);
			glVertex2i(0, h);
			glEnd();
		}else {
			glDisable(GL_TEXTURE_2D);

			glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(w, 0);
			glVertex2i(w, h);
			glVertex2i(0, h);
			glEnd();

			glEnable(GL_TEXTURE_2D);
		}

		glPopMatrix();
	}

public:
	std::function<void(VisualComponent2D&)> onClick;    ///< Fonction à appeler quand on clique sur le composant.
	std::function<void(VisualComponent2D&)> onMouseOver;    ///< Fonction à appeler quand le curseur entre le composant.
	std::function<void(VisualComponent2D&)> onMouseAway;    ///< Fonction à appeler quand le curseur sort du composant.
	unsigned int* texIds;    ///< Tableau d'id des textures utilisées par le composant.
	unsigned int currTexIndex;    ///< Indice de l'élément de texIds à utiliser dans draw().
	Position pos;    ///< Structure donnant la position en pixel du VisualComponent2D relatif à son parent.
	int w;    ///< Largeur en pixel du VisualComponent2D sur l'écran.
	int h;    ///< Hauteur en pixel du VisualComponent2D sur l'écran.
	int priority;    ///< Couche sur laquelle le composant sera dessiné. (-1 en dessous de 0)
	bool catchesEvents;    ///< Vrai si le composant réagit aux événements.
	bool visibility;    ///< Vrai si le composant est dessiné.
	double angle;    ///< Angle en radian du composant.
	std::pair<int, int> pivot;    ///< Position relative au composant autour duquel le composant est rotaté.

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param w Valeur initiale de la composante w.
	/// \param h Valeur initiale de la composante h.
	/// \param catchesEvents Valeur initiale de catchesEvents.
	/// \param argc Nombre d'id de texture à ajouter.
	/// \param ... Ids de texture à ajouter.
	VisualComponent2D(const int& priority, const int& x, const int& y, const int& w, const int& h, const bool& catchesEvents, int argc, ...)
			: pos(x, y), priority(priority), w(w), h(h), catchesEvents(catchesEvents), currTexIndex(0), visibility(true), angle(0.), pivot(std::make_pair(w / 2, h / 2)) {
		if (argc) {
			texIds = new unsigned int[argc];

			va_list argv;
			va_start(argv, argc);
			for (unsigned int i = 0; i < argc; i++)
				texIds[i] = va_arg(argv, unsigned
						int);
			va_end(argv);
		}else
			texIds = nullptr;
	}

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param w Valeur initiale de la composante w.
	/// \param h Valeur initiale de la composante h.
	/// \param catchesEvents Valeur initiale de catchesEvents.
	/// \param textureId Id de texture à ajouter.
	VisualComponent2D(const int& priority, const int& x, const int& y, const int& w, const int& h, const bool& catchesEvents, const unsigned int& textureId)
			: VisualComponent2D(priority, x, y, w, h, catchesEvents, 1, textureId) {}

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param w Valeur initiale de la composante w.
	/// \param h Valeur initiale de la composante h.
	/// \param catchesEvents Valeur initiale de catchesEvents.
	VisualComponent2D(const int& priority, const int& x, const int& y, const int& w, const int& h, const bool& catchesEvents = false)
			: VisualComponent2D(priority, x, y, w, h, catchesEvents, 0) {}

	virtual ~VisualComponent2D() {
		if (onClick)
			EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
		if (onMouseOver || onMouseAway)
			EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, this);

		if (texIds)
			delete[] texIds;
	}

	/// \brief Affectation de la taille.
	/// \param width Valeur de la largeur.
	/// \param height Valeur de la hauteur.
	void setSize(const int& width, const int& height) {
		w = width;
		h = height;
	}

	/// \brief Incrémentation de la taille.
	/// \param dw Valeur à ajouter à la largeur.
	/// \param dh Valeur à ajouter à la hauteur.
	void increaseScaleBy(const int& dw, const int& dh) {
		w += dw;
		h += dh;
	}

	/// \brief Mise à l'échelle de la taille.
	/// \param w Valeur à multiplier à la largeur.
	/// \param h Valeur à multiplier à la hauteur.
	void scaleBy(const int& w, const int& h) {
		this->w *= w;
		this->h *= h;
	}

	/// \return Obtention de mouseIsOver
	const bool& isMouseOver() const {
		return mouseIsOver;
	}

	/// \brief Dessine le composant.
	virtual void draw() {
		drawQuad(true);
	};

	/// \brief Réagit à l'évévement décrit dans event.
	/// \param event Structure contenant l'information sur l'événement.
	virtual void notify(SDL_Event* event) {
		if (catchesEvents) {
			if (pos.containsPoint(w, h, event->motion.x, event->motion.y, angle, pivot)) {
				switch (event->type) {
					case SDL_MOUSEBUTTONUP:
						if (onClick)
							onClick(*this);
						break;
					case SDL_MOUSEMOTION:
						if (onMouseOver && !mouseIsOver)
							onMouseOver(*this);
						mouseIsOver = true;
						break;
					default:;
				}
			}else if (event->type == SDL_MOUSEMOTION) {
				if (mouseIsOver && onMouseAway)
					onMouseAway(*this);
				mouseIsOver = false;
			}
		}
	}
};

#endif //SOURCE_VISUALCOMPONENT2D_H
