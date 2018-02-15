#ifndef SOURCE_AABB_H
#define SOURCE_AABB_H

#include "Collidable.h"
#include "Vector3d.h"

enum class Side : unsigned char {
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

///\brief Boîte alignée aux axes globales définie par deux points

struct AABB {
	Vector3d min;	///< Sommet aux composantes minimales
	Vector3d max;	///< Sommet aux composantes maximales

	AABB() = default;

	AABB(const Vector3d& min, const Vector3d& max) : min(min), max(max) {}

	AABB(Vector3d&& min, Vector3d&& max) : min(std::move(min)), max(std::move(max)) {}

	AABB(const AABB& boundingBox) = default;

	AABB(AABB&& boundingBox) = default;

	AABB& operator=(const AABB& boundingBox) = default;

	AABB& operator=(AABB&& boundingBox) = default;

	/// \brief Permet de savoir si un vecteur intersecte la AABB
	///
	///	Version adaptée de l'algorithm "Fast Ray-Box Intersection" par Andrew Woo
	/// https://web.archive.org/web/20090803054252/http://tog.acm.org/resources/GraphicsGems/gems/RayBox.c
	///
	/// \param direction Vecteur de direction représentant aussi la longueur
	/// \param position Position du vecteur
	/// \return Vrai si le vecteur intersecte
	bool intersects(const bool& infiniteLength, const Vector3d& direction, const Vector3d& position) const {
		bool inside = true;
		Side quadrant[3];
		unsigned whichPlane;
		double maxT[3];
		double candidatePlane[3];
		double coord[3];

		for (unsigned i = 0; i < 3; i++)
			if (position.get(i) < min.get(i)) {
				quadrant[i] = Side::LEFT;
				candidatePlane[i] = min.get(i);
				inside = false;
			}else if (position.get(i) > max.get(i)) {
				quadrant[i] = Side::RIGHT;
				candidatePlane[i] = max.get(i);
				inside = false;
			}else
				quadrant[i] = Side::MIDDLE;

		if (inside) {
			coord[0] = position.x;
			coord[1] = position.y;
			coord[2] = position.z;
			return true;
		}

		for (unsigned i = 0; i < 3; i++)
			if (quadrant[i] != Side::MIDDLE && direction.get(i) != 0.)
				maxT[i] = (candidatePlane[i] - position.get(i)) / direction.get(i);
			else
				maxT[i] = -1.;

		whichPlane = 0;
		for (unsigned i = 1; i < 3; i++)
			if (maxT[whichPlane] < maxT[i])
				whichPlane = i;

		if (maxT[whichPlane] < 0.)
			return false;

		for (unsigned i = 0; i < 3; i++)
			if (whichPlane != i) {
				coord[i] = position.get(i) + maxT[whichPlane] * direction.get(i);

				if (coord[i] < min.get(i) || coord[i] > max.get(i))
					return false;
			}else
				coord[i] = candidatePlane[i];

		if (infiniteLength)
			return true;

		return Math::isNearlyLessOrEqual(maxT[0], 1., VERY_SMALL_DOUBLE) &&
			   Math::isNearlyLessOrEqual(maxT[1], 1., VERY_SMALL_DOUBLE) &&
			   Math::isNearlyLessOrEqual(maxT[2], 1., VERY_SMALL_DOUBLE);
	}

	/// \brief Dessine les arêtes de la aabb, en vert par défaut
	void draw(const Vector3d& color = Vector3d::UP) const {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor3d(color.x, color.y, color.z);

		glBegin(GL_LINES);
		glVertex3d(min.x, min.y, min.z);
		glVertex3d(max.x, min.y, min.z);

		glVertex3d(min.x, min.y, max.z);
		glVertex3d(max.x, min.y, max.z);

		glVertex3d(min.x, max.y, min.z);
		glVertex3d(max.x, max.y, min.z);

		glVertex3d(min.x, max.y, max.z);
		glVertex3d(max.x, max.y, max.z);

		glVertex3d(min.x, min.y, min.z);
		glVertex3d(min.x, max.y, min.z);

		glVertex3d(min.x, min.y, max.z);
		glVertex3d(min.x, max.y, max.z);

		glVertex3d(max.x, min.y, min.z);
		glVertex3d(max.x, max.y, min.z);

		glVertex3d(max.x, min.y, max.z);
		glVertex3d(max.x, max.y, max.z);

		glVertex3d(min.x, min.y, min.z);
		glVertex3d(min.x, min.y, max.z);

		glVertex3d(max.x, min.y, min.z);
		glVertex3d(max.x, min.y, max.z);

		glVertex3d(min.x, max.y, min.z);
		glVertex3d(min.x, max.y, max.z);

		glVertex3d(max.x, max.y, min.z);
		glVertex3d(max.x, max.y, max.z);
		glEnd();

		glColor3d(1., 1., 1.);

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
};

#endif //SOURCE_AABB_H