#ifndef SOURCE_DECORMODEL_H
#define SOURCE_DECORMODEL_H

#include "Model.h"
#include "Collidable.h"

class DecorModel : public Model, public Collidable {
private:
	/// \return Un tableau de composantes de sommets
	const double* getVertices() const {
		return mesh.getVertices();
	}

	/// \return Le nombre de sommets
	const unsigned int& getVertexCount() const {
		return mesh.getVertexCount();
	}

public:
	DecorModel(const char* fileName, const unsigned int& textureID) : Model(fileName, textureID, true) {}

	/// \brief Permet de savoir si un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	/// \param direction Vecteur de direction
	/// \param position Position du vecteur
	/// \return Point d'intersection le plus proche entre le vecteur et le mesh et le r minimal
	Intersect isColliding(const bool& infiniteLength, const Vector3d& direction, const Vector3d& position) const {
		return mesh.getAABB().intersects(infiniteLength, direction, position)
			   ? isCollidingVector(infiniteLength, mesh.getVertexCount(), mesh.getVertices(), mesh.getNormals(), direction, position)
			   : Intersect();
	}
};

#endif
