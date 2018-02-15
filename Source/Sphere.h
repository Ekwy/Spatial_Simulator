#ifndef SOURCE_SPHERE_H
#define SOURCE_SPHERE_H

#include "Model.h"
#include "Vector3d.h"

class Sphere {
private:
	Model model;    ///< Sphère
	double radius;   ///< Rayon de la sphère

public:
	Sphere(const double& radius, const unsigned int& textureID, const bool& applyLight) : radius(radius), model("Textures3D/sphere.obj", textureID, applyLight) {
		model.scale(radius);
	}

	/// \brief Change la couleur de la sphère
	/// \param r Pourcentage de rouge
	/// \param g Pourcentage de vert
	/// \param b Pourcentage de bleu
	void setColor(const double& r, const double& g, const double& b) {
		model.setColor(r, g, b);
	}

	/// \brief Grandit le modèle de la Sphere d'un facteur scaleFactor
	/// \param scaleFactor Facteur de grandissement
	void scale(double scaleFactor) {
		model.scale(scaleFactor);
		radius *= scaleFactor;
	}

	/// \brief Permet la rotation de la sphère autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	virtual void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		model.rotateAroundLine(axis, point, angle);
	}

	/// \brief Déplace la sphère
	/// \param movement Déplacement en x,y,z
	void move(const Vector3d& movement) {
		move(movement.x, movement.y, movement.z);
	}

	/// \brief Déplace la sphère
	/// \param x Déplacement en x
	/// \param y Déplacement en y
	/// \param z Déplacement en z
	void move(double x, double y, double z) {
		model.move(x, y, z);
	}

	/// \brief Détecte si une collision a lieu avec une autre sphère
	/// \param sphere Sphère avec qui on teste la collision
	/// \return Booléen true s'il y a collision
	bool isColliding(Sphere& sphere) {
		if (getPos() - sphere.getPos() <= radius + sphere.getRadius())
			return true;
		return false;
	}


	/// \brief Accès à la position de la sphère
	/// \return Constante en référence de la position de la sphère
	const Vector3d getPos() const {
		return model.getPos();
	}

	/// \brief Accès au model de la sphère
	/// \return Référence au modèle de la sphère
	Model& getModel() {
		return model;
	}

	///< \return Un tableau de composantes de sommets
	const double& getVertices() const {
		return *model.getMesh().getVertices();
	}

	/// \return Le nombre de sommets
	const unsigned int& getVertexCount() const {
		return model.getMesh().getVertexCount();
	}

	/// \return Les normals de la sphère
	const double& getNormals() const {
		return *model.getMesh().getNormals();
	}

	/// \return Le rayon de la sphère
	const double& getRadius() {
		return radius;
	}

	/// \return Le Axis-Aligned Boundary-Box de la sphère
	const AABB& getAABB() const {
		return model.getMesh().getAABB();
	}

	/// \brief Dessine la sphère
	void draw() {
		model.draw();
	}
};

#endif
