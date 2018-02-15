#ifndef SOURCE_COLLIDABLE_H
#define SOURCE_COLLIDABLE_H

struct Intersect {
	bool collided;	///< Vrai si il y a eu collision
	Vector3d pos;	///< Position de la collision
	double r;	///< Fraction coupée du vecteur direction par la collision

	Intersect() : collided(false) {}
	Intersect(const bool& collided, const Vector3d& pos, const double& r) : collided(collided), pos(pos), r(r) {}
	Intersect(bool&& collided, Vector3d&& pos, double&& r) : collided(std::move(collided)), pos(std::move(pos)), r(std::move(r)) {}
};

class Collidable {
protected:
	/// \brief Permet de savoir si à un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	/// \param direction Vecteur de direction
	/// \param position Position du vecteur
	/// \return Point d'intersection le plus proche entre le vecteur et le mesh ainsi que le r minimal
	Intersect isCollidingVector(const bool& infiniteLength, const unsigned int& vertexCount, const double* vertices, const double* normals, const Vector3d& direction, const Vector3d& position) const {
		for (unsigned int i = 0; i < vertexCount; i += 3)
			if (normals[i * 3] * direction.x + normals[i * 3 + 1] * direction.y + normals[i * 3 + 2] * direction.z < 0) {
				double r = (normals[i * 3] * (vertices[i * 3] - position.x) + normals[i * 3 + 1] * (vertices[i * 3 + 1] - position.y) + normals[i * 3 + 2] * (vertices[i * 3 + 2] - position.z)) / (normals[i * 3] * direction.x + normals[i * 3 + 1] * direction.y + normals[i * 3 + 2] * direction.z);
				if (Math::isNearlyGreaterOrEqual(r, 0., SMALL_DOUBLE) && (infiniteLength || Math::isNearlyLessOrEqual(r, 1.))) {
					Vector3d intersect(position + r * direction);

					Vector3d v1(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
					Vector3d v2(vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1], vertices[(i + 1) * 3 + 2]);
					Vector3d v3(vertices[(i + 2) * 3], vertices[(i + 2) * 3 + 1], vertices[(i + 2) * 3 + 2]);

					if (Math::isNearlyGreaterOrEqual(((v3 - v2) / (intersect - v2)) * ((v3 - v2) / (v1 - v2)), 0., SMALL_DOUBLE) &&
						Math::isNearlyGreaterOrEqual(((v3 - v1) / (intersect - v1)) * ((v3 - v1) / (v2 - v1)), 0., SMALL_DOUBLE) &&
						Math::isNearlyGreaterOrEqual(((v2 - v1) / (intersect - v1)) * ((v2 - v1) / (v3 - v1)), 0., SMALL_DOUBLE))
						return {true, intersect, r};
				}
			}

		return Intersect();
	}

	/// \brief Permet de savoir si à un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	/// \param direction Vecteur de direction
	/// \param position Position du vecteur
	/// \return Point d'intersection le plus proche entre le vecteur et le mesh ainsi que le r minimal
	Intersect isCollidingVector(const bool& infiniteLength, const unsigned int& vertexCount, const unsigned int* indices, const double* vertices, const double* normals, const Vector3d& direction, const Vector3d& position) const {
		for (unsigned int i = 0; i < vertexCount; i += 3)
			if (normals[indices[i] * 3] * direction.x + normals[indices[i] * 3 + 1] * direction.y + normals[indices[i] * 3 + 2] * direction.z < 0) {
				double r = (normals[indices[i] * 3] * (vertices[indices[i] * 3] - position.x) + normals[indices[i] * 3 + 1] * (vertices[indices[i] * 3 + 1] - position.y) + normals[indices[i] * 3 + 2] * (vertices[indices[i] * 3 + 2] - position.z)) / (normals[indices[i] * 3] * direction.x + normals[indices[i] * 3 + 1] * direction.y + normals[indices[i] * 3 + 2] * direction.z);
				if (Math::isNearlyGreaterOrEqual(r, 0., SMALL_DOUBLE) && (infiniteLength || Math::isNearlyLessOrEqual(r, 1.))) {
					Vector3d intersect(position + r * direction);

					Vector3d v1(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
					Vector3d v2(vertices[(indices[i] + 1) * 3], vertices[(indices[i] + 1) * 3 + 1], vertices[(indices[i] + 1) * 3 + 2]);
					Vector3d v3(vertices[(indices[i] + 2) * 3], vertices[(indices[i] + 2) * 3 + 1], vertices[(indices[i] + 2) * 3 + 2]);

					if (Math::isNearlyGreaterOrEqual(((v3 - v2) / (intersect - v2)) * ((v3 - v2) / (v1 - v2)), 0., SMALL_DOUBLE) &&
						Math::isNearlyGreaterOrEqual(((v3 - v1) / (intersect - v1)) * ((v3 - v1) / (v2 - v1)), 0., SMALL_DOUBLE) &&
						Math::isNearlyGreaterOrEqual(((v2 - v1) / (intersect - v1)) * ((v2 - v1) / (v3 - v1)), 0., SMALL_DOUBLE))
						return {true, intersect, r};
				}
			}

		return Intersect();
	}

	/// \return Un tableau de composantes de sommets
	virtual const double* getVertices() const = 0;

	/// \return Le nombre de sommets
	virtual const unsigned int& getVertexCount() const = 0;

public:
	/// \brief Modifie le vecteur movement pour refléter le mouvement maximal du Collidable
	/// \param obj Collidable auquel tester la collision
	/// \param movement Mouvement maximal désiré
	virtual double findMaxMovement(const Collidable& obj, const Vector3d& movement) const {
		double maxDistance(1.);

		for (unsigned int i = 0; i < getVertexCount() * 3; i += 3) {
			Intersect intersect(obj.isColliding(false, movement, {getVertices()[i], getVertices()[i + 1], getVertices()[i + 2]}));
			if (intersect.collided)
				if (!Math::areNearlyEqual(intersect.r, 0., VERY_SMALL_DOUBLE))
					maxDistance = std::min(maxDistance, intersect.r);
		}

		return maxDistance;
	}

	/// \brief Permet de savoir si un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	/// \param direction Vecteur de direction
	/// \param position Position du vecteur
	/// \return Point d'intersection le plus proche entre le vecteur et le mesh et le r minimal
	virtual Intersect isColliding(const bool& infiniteLength, const Vector3d& direction, const Vector3d& position) const = 0;
};

#endif //SOURCE_COLLIDABLE_H
