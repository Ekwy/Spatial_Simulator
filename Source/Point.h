#ifndef SOURCE_POINT_H
#define SOURCE_POINT_H

/// \brief Affiche une ligne en 3d

#include "Mesh.h"
#include "Vector3d.h"

struct Point {
	Vector3d pos;    ///< Vecteur a du point
	Vector3d color;    ///< Vecteur de couleur du point
	double size;	///< Taille du point

	Point(const Vector3d& pos, const Vector3d& color, const double& size = 1.) : pos(pos), color(color), size(size) {}

	/// \brief Dessine le point
	void draw() const {
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glPointSize(size);

		glBegin(GL_POINTS);
		glColor3d(color.x, color.y, color.z);
		glVertex3d(pos.x, pos.y, pos.z);
		glColor3d(1.0, 1.0, 1.0);
		glEnd();

		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};

#endif //SOURCE_POINT_H
