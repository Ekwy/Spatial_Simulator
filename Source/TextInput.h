/// \brief Afficher une saisie de texte en SDL
///
/// Classe représentant une boîte de saisie de texte à l'aide de SDL [Visual Component 2D]
///

#ifndef SDLENGINE_TEXTINPUT_H
#define SDLENGINE_TEXTINPUT_H

#include <string>
#include "Label.h"

class TextInput : public Label {
private:
	bool focused; ///< Curseur engagé ou non (écriture ou pas écriture)?

public:
	std::function<void(VisualComponent2D&)> onFocus; ///< Pointeur de fonction lors du focus de l'objet.
	std::function<void(VisualComponent2D&)> onUnfocus; ///< Pointeur de fonction lors du dé-focus de l'objet.
	std::function<void(VisualComponent2D&)> onEdit; ///< Pointeur de fonction lors de l'édition du texte.

	/// \param text Texte par défaut (?).
	/// \param fontName Police d'écriture de la saisie de texte.
	/// \param priority Priorité d'affichage.
	TextInput(int priority, double x, double y, double w, double h, const std::string& text, const char* fontName, double size, SDL_Color color) : Label(priority, x, y, text, fontName, size, color), focused(false) {
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONUP, this);
		EventDispatcher::registerEventObserver(SDL_TEXTINPUT, this);
		EventDispatcher::registerEventObserver(SDL_KEYUP, this);
	}

	~TextInput() {
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
		EventDispatcher::unregisterEventObserver(SDL_TEXTINPUT, this);
		EventDispatcher::unregisterEventObserver(SDL_KEYUP, this);
	}

	/// \brief Enregistre les évênements claviers pour update le texte.
	/// \param event Structure d'évênement.
	void notify(SDL_Event* event) {
		//si curseur a l'interieur du rect
		switch (event->type) {
			case SDL_TEXTINPUT:
				if (focused) {
					text += event->text.text;
					if (onEdit)
						onEdit(*this);
					updateTexture();
				}
				break;
			case SDL_KEYUP:
				if (event->key.keysym.sym == SDLK_BACKSPACE && focused) {
					text = text.substr(0, text.size() - 1);
					if (onEdit)
						onEdit(*this);
					updateTexture();
				}
				break;
			default:;
		}
		if (pos.containsPoint(w, h, event->motion.x, event->motion.y)) {
			switch (event->type) {
				case SDL_MOUSEBUTTONUP:
					if (onClick)
						onClick(*this);
					if (onFocus)
						onFocus(*this);
					focused = true;
					SDL_StartTextInput();
					break;
				case SDL_MOUSEMOTION:
					if (onMouseOver && !mouseIsOver)
						onMouseOver(*this);
					mouseIsOver = true;
				default:;
			}
		}else {
			switch (event->type) {
				case SDL_MOUSEBUTTONUP:
					if (onUnfocus)
						onUnfocus;
					focused = false;
					break;
				case SDL_MOUSEMOTION:
					if (mouseIsOver && onMouseAway)
						onMouseAway(*this);
					mouseIsOver = false;
				default:;
			}
		}
	}
};

#endif //SDLENGINE_TEXTINPUT_H
