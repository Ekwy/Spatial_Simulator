#ifndef SOURCE_BUTTON_H
#define SOURCE_BUTTON_H

/// \brief Composant visuel 2d réagissant au clique et déclique.

#include <SDL2/SDL_opengl.h>
#include "SDLGLContext.h"
#include "VisualComponent2D.h"
#include "EventDispatcher.h"

class Button : public VisualComponent2D {
public:
	std::function<void(VisualComponent2D&)> onPress;    ///< Fonction appelée quand le bouton est appuyé.
	std::function<void(VisualComponent2D&)> onUnPress;    ///< Fonction appelée quand le bouton est relâché.
	unsigned button;	///< Bouton de souris avec lequel le bouton doit être appuyé, 0 pour tous, bouton de gauche par défaut

	/// \param textureIdUnpressed Id texture du bouton non-pressé.
	/// \param textureIdPressed Id texture du bouton pressé.
	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param w Valeur initiale de la composante w.
	/// \param h Valeur initiale de la composante h.
	Button(const unsigned int& textureIdUnpressed, const unsigned int& textureIdPressed, const int& priority, const int& x, const int& y, const int& w, const int& h) : VisualComponent2D(priority, x, y, w, h, true, 2, textureIdUnpressed, textureIdPressed), button(1) {
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONUP, this);
	}

	/// \param textureIdUnpressed Id texture du bouton non-pressé.
	/// \param textureIdPressed Id texture du bouton pressé.
	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	Button(const unsigned int& textureIdUnpressed, const unsigned int &textureIdPressed, const int& priority, const int& x, const int& y) : VisualComponent2D(priority, x, y, 0, 0, true, 2, textureIdUnpressed, textureIdPressed), button(1) {
		glBindTexture(GL_TEXTURE_2D, textureIdUnpressed);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONUP, this);
	}

	virtual ~Button() {
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
	}

	/// \brief Dessine le Button avec la texture texIds[currTexIndex].
	void draw() {
		drawQuad(true);
	}

	/// \brief Réagit à l'évévement décrit dans event.
	/// \param event Structure contenant l'information sur l'événement.
	virtual void notify(SDL_Event* event) {
		if (catchesEvents) {
			if (pos.containsPoint(w, h, event->motion.x, event->motion.y, angle, pivot)) {
				switch (event->type) {
					case SDL_MOUSEBUTTONUP:
						if (currTexIndex && (button ? event->button.button == button : true)) {	//Ici, currTexIndex est utilisé pour déterminer si le bouton est pressé.
							if (onClick)
								onClick(*this);
							if (onUnPress)
								onUnPress(*this);
							currTexIndex = 0;
						}
						break;
					case SDL_MOUSEMOTION:
						if (onMouseOver && !mouseIsOver)
							onMouseOver(*this);
						mouseIsOver = true;
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (button ? event->button.button == button : true) {
							if (onPress)
								onPress(*this);
							currTexIndex = 1;
						}
						break;
					default:;

				}
			}else {
				if (event->type == SDL_MOUSEBUTTONUP && currTexIndex && (button ? event->button.button == button : true)) {
					if (onUnPress)
						onUnPress(*this);
					currTexIndex = 0;
				}
				if (mouseIsOver && onMouseAway)
					onMouseAway(*this);
				mouseIsOver = false;
			}
		}
	}
};

#endif //SOURCE_BUTTON_H
