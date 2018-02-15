/// \brief Afficher un rectangle en OpenGL
///
/// Classe représentant un rectangle à l'aide d'OpenGL [Visual Component 2D]
///

#ifndef SOURCE_RECT_H
#define SOURCE_RECT_H

#include <SDL2/SDL_opengl.h>
#include "VisualComponent2D.h"
#include "Vector3d.h"

class Rect : public VisualComponent2D {
public:
	bool wireframe; ///< Rectangle plein ou vide?
	Vector3d color; ///< Couleur du rectangle.

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param w Valeur initiale de la composante w.
	/// \param h Valeur initiale de la composante h.
	/// \param catchesEvents Valeur initiale de catchesEvents.
	/// \param color Couleur du Rect sous forme de vecteur 3d aux composantes entre 0.0 et 1.0
	/// \param wireframe Vrai si le Rect doit être vide.
	Rect(const int& priority, const int& x, const int& y, const int& w, const int& h, const bool& catchesEvents, const Vector3d& color, const bool& wireframe = false)
			: VisualComponent2D(priority, x, y, w, h, catchesEvents), wireframe(wireframe), color(color) {}

	/// \brief Dessine le Rect.
	void draw() {
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glColor4d(color.x, color.y, color.z, 0.5);

		drawQuad(false);

		glColor3d(1., 1., 1.);

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};

#endif //SOURCE_RECT_H
