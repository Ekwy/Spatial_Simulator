///\brief Objet positionné et orienté dans un environnemnt 3D
///
///Classe permettant d'afficher un mesh dans un environnement 3D selon une position et une orientation dans l'espace
///

#ifndef SOURCE_MODEL_H
#define SOURCE_MODEL_H

#include "Mesh.h"
#include "Vector3d.h"
#include "Matrix4d.h"
#include "Observer.h"
#include "EventDispatcher.h"
#include "SDLGLContext.h"
#include "Collidable.h"

class Model {
private:
	Matrix4d transform; ///< Matrice de transformation

protected:
	Mesh mesh; ///< Squelette de l'objet 3D

public:
	/// \param fileName Fichier texte contenant les caractéristiques du mesh
	/// \param textureID ID de la texture à appliquer sur l'objet
	/// \param hasHitbox Si vrai, crée la hitbox
	Model(const char* fileName, const unsigned int& textureID, const bool& applyLight) : mesh(fileName, textureID, applyLight) {
		transform.setIdentity();
	}

	/// \brief Accession
	/// \return Référence constante à mesh
	const Mesh& getMesh() const {
		return mesh;
	}

	/// \return transform
	const Matrix4d& getTransform() const {
		return transform;
	}

	/// \brief Obtention du vecteur position
	/// \return Référence constante du vecteur position
	Vector3d getPos() const {
		return transform.getPos();
	}

	/// \brief Applique les transformation d'une matrice 4x4 au model
	/// \param Matrice de transformation
	void applyTransform(const Matrix4d& mat) {
		mesh.transform(mat);
		transform = mat * transform;
	}

	/// \brief Fonction permettant le mouvement du model
	/// \param x Mouvement en x
	/// \param y Mouvement en y
	/// \param z Mouvement en z
	virtual void move(double x, double y, double z) {
		mesh.move(x, y, z);
		transform.translate(x, y, z);
	}

	/// \brief Fonction permettant le mouvement du modèle
	/// \param movement Vecteur mouvement
	virtual void move(const Vector3d& movement) {
		mesh.move(movement);
		transform.translate(movement);
	}

	/// \brief Permet la rotation du modèle autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		Matrix4d rot = Matrix4d().setRotationAroundLine(axis, point, angle);

		mesh.transform(rot);
		transform = rot * transform;
	}

	/// \brief Fonction permettant un scaling du model
	/// \param scaleFactor Facteur de grandissement
	void scale(double scaleFactor) {
		mesh.move(-transform.getPos());
		mesh.scale(scaleFactor, scaleFactor, scaleFactor);
		mesh.move(transform.getPos());
	}

	/// \brief Fonction permettant un scaling du model
	/// \param x Scale en x
	/// \param y Scale en y
	/// \param z Scale en z
	void scale(double x, double y, double z) {
		mesh.move(-transform.getPos());
		mesh.scale(x, y, z);
		mesh.move(transform.getPos());
	}

	/// \brief Fonction permettant un scaling du model à l'aide d'un Vecteur
	/// \param scaling Vecteur pour le scale
	void scale(const Vector3d& scaling) {
		mesh.scale(scaling.x, scaling.y, scaling.z);
	}

	/// \brief Fonction permettant l'affichage du model
	virtual void draw() const {
		mesh.draw();
	}

	/// \brief Affectation de la couleur du mesh.
	/// \param x Valeur de la composante r.
	/// \param y Valeur de la composante g.
	/// \param z Valeur de la composante b.
	void setColor(const double& r, const double& g, const double& b) {
		mesh.setColor({r,g,b});
	}
};

#endif
