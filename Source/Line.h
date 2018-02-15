#ifndef SOURCE_LINE_H
#define SOURCE_LINE_H

/// \brief Affiche une ligne en 3d

#include "Mesh.h"

struct Line {
	Vector3d a;	///< Vecteur a de la ligne
	Vector3d b;	///< Vecteur b de la ligne
	Vector3d color;	///< Vecteur de couleur de la ligne

	Line() = default;

	Line(const Vector3d& a, const Vector3d& b, const Vector3d& color) : a(a), b(b), color(color) {}

	Line(Vector3d&& a, Vector3d&& b, Vector3d&& color) : a(std::move(a)), b(std::move(b)), color(std::move(color)) {}

	/// \brief Déplace la ligne par le vecteur movement
	void move(const Vector3d& movement) {
		a += movement;
		b += movement;
	}

	/// \brief Dessine la ligne
	void draw() const {
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glBegin(GL_LINES);
			glColor3d(color.x, color.y, color.z);
			glVertex3d(a.x, a.y, a.z);
			glVertex3d(b.x, b.y, b.z);
			glColor3d(1., 1., 1.);
		glEnd();

		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};

#endif //SOURCE_LINE_H
