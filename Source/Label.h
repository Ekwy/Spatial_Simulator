/// \brief Afficher du texte en SDL.
///
/// Classe permettant d'écrire à l'aide d'une police dans une fenêtre SDL  [Visual Component 2D].
///

#ifndef SDLENGINE_LABEL_H
#define SDLENGINE_LABEL_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "VisualComponent2D.h"

class Label : public VisualComponent2D {
protected:
	std::string text; ///< Texte à afficher
	const char* fontName; ///< Nom de la police d'écriture
	SDL_Color color; ///< Couleur du texte
	int size; ///< Taille des caractères

public:
	Label(int priority, double x, double y, std::string text, const char* fontName, int size, SDL_Color color) : VisualComponent2D(priority, x, y, 0, 0, true), text(text), size(size), fontName(fontName), color(color) {
		color.b = color.b ^ color.r;
		color.r = color.b ^ color.r;
		color.b = color.b ^ color.r;

		texIds = new unsigned int({0});

		updateTexture();
	}

	/// \brief Changer la couleur du texte
	/// \param r Rouge
	/// \param g Vert
	/// \param b Bleu
	/// \param a Transparence
	void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;

		updateTexture();
	}

	/// \brief Obtenir le texte du label
	/// \return Retourne une référence constante vers le texte du label
	const std::string& getStr() {
		return text;
	}

	/// \brief Changer le texte du label
	/// \param str Nouveau texte
	void setStr(std::string str) {
		this->text = str;
		updateTexture();
	}

	/// \brief Met la texture du texte à jour après une modification
	void updateTexture() {
		texIds[currTexIndex] = SDLGLContext::loadLabel(text, fontName, color, size, texIds[currTexIndex]);
		TTF_Font* font(TTF_OpenFont(fontName, size));
		TTF_SizeText(font, text.c_str(), &w, &h);
		TTF_CloseFont(font);
	}

	/// \brief Dessine le Label avec la texture texIds[currTexIndex].
	void draw() {
		drawQuad(true);
	}
};

#endif //SDLENGINE_LABEL_H
