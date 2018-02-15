/// \brief Case à cocher en 2D
///
/// Classe représentant une case à cocher à l'aide d'OpenGL [Visual Component 2D]
///

#ifndef SOURCE_CHECKBOX_H
#define SOURCE_CHECKBOX_H

#include <SDL2/SDL_ttf.h>
#include <functional>
#include "Button.h"

class CheckBox : public Button {
private:
	bool state; ///< Indique si le bouton est pressé
	bool mousePressed; ///< Vrai si le curseur était dans le VisComp quand le bouton a été appuyé

public:
	/// \param textureIdUnchecked Id texture du bouton non-pressé.
	/// \param textureIdChecked Id texture du bouton pressé.
	/// \param priority Valeur initiale de priority.
	/// \param state Valeur initiale de state
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param w Valeur initiale de la composante w.
	/// \param h Valeur initiale de la composante h.
	CheckBox(unsigned int textureIdUnchecked, unsigned int textureIdChecked, int priority, bool state, int x, int y, int w, int h) : Button(textureIdUnchecked, textureIdChecked, priority, x, y, w, h), state(state), mousePressed(false) {
		currTexIndex = state;
	}

	/// \brief Dessine le CheckBox avec la texture texIds[currTexIndex].
	void draw() {
		drawQuad(true);
	}

	/// \brief Réagit à l'évévement décrit dans event.
	/// \param event Structure contenant l'information sur l'événement.
	void notify(SDL_Event* event) {
		if (catchesEvents)
			switch(event->type) {
				case SDL_MOUSEBUTTONDOWN:
					if (button ? event->button.button == button : true)
						if (pos.containsPoint(w, h, event->button.x, event->button.y))
							mousePressed = true;
					break;
				case SDL_MOUSEBUTTONUP:
					if (button ? event->button.button == button : true) {
						if (pos.containsPoint(w, h, event->button.x, event->button.y)) {
							if (mousePressed) {
								if (state = !state) {
									if (onClick)
										onClick(*this);
									if (onPress)
										onPress(*this);
									currTexIndex = 1;
									state = true;
								}else {
									if (onUnPress)
										onUnPress(*this);
									currTexIndex = 0;
									state = false;
								}
							}
						}

						mousePressed = false;
					}
				default:;
			}
	}

	/// \brief Donne l'état de la check box.
	/// \return Retourne l'état de la check box.
	bool getState() {
		return state;
	}
};

#endif //SOURCE_CHECKBOX_H
