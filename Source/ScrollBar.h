/// \brief Barre de défilememt
///
/// Classe qui fonctionne comme un boutton, mais qui permet en plus de défiler un visuel component auquel il est lié.
///

#ifndef SOURCE_SCROLLBAR_H
#define SOURCE_SCROLLBAR_H

#include <SDL2/SDL_opengl.h>
#include "SDLGLContext.h"
#include "Button.h"
#include "EventDispatcher.h"

class ScrollBar : public Button {
public:
	std::function<void(VisualComponent2D&)> onMouseWheelUp; ///< Fonction utilisé lors d'un défilement avec la molette
	std::function<void(VisualComponent2D&)> onMouseWheelDown; ///< Fonction utilisé lors d'un défilement avec la molette

	/// \param panelPos Valeur initiale de panelPos.
	/// \param panelAnimPos Valeur initiale de panelAnimPos.
	ScrollBar(const unsigned int& textureIdUnpressed, const unsigned int& textureIdPressed, const int& priority, const bool& catchesEvents, const int& x, const int& y, const int& w, const int& h)
			: Button(textureIdUnpressed, textureIdPressed, priority, x, y, w, h) {
		EventDispatcher::registerEventObserver(SDL_MOUSEWHEEL, this);
	}

	~ScrollBar() {
		EventDispatcher::unregisterEventObserver(SDL_MOUSEWHEEL, this);
	}

	/// \brief Effectue la fonction pointé par le pointeur de fonction en fonction du dernier évènement
	/// \param onMouseWheel Pointeur de la fonction désiré.
	/// \return rien pantoute
	void notify(SDL_Event* event) {
		if (pos.containsPoint(w, h, event->motion.x, event->motion.y)) {
			switch (event->type) {
				case SDL_MOUSEBUTTONUP:
					if (currTexIndex && (button ? event->button.button == button : true)) {
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

		if (event->type == SDL_MOUSEWHEEL)
			if (event->wheel.y < 0)
				onMouseWheelDown(*this);
			else
				onMouseWheelUp(*this);
	}

	/// \brief Dessine le ScrollBar avec la texture texIds[currTexIndex].
	void draw() {
		drawQuad(true);
	}
};
#endif //SOURCE_SCROLLBAR_H
