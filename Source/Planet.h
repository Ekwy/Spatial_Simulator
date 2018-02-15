/// \brief Planète dans un monde 3D décrit par une sphère
///
///
#ifndef SOURCE_model_H
#define SOURCE_model_H

#include "Model.h"
#include "PhysicObject.h"
#include "Sphere.h"
#include "Cockpit.h"
#include "ModelGroup.h"
#include "DecorModel.h"
#include <list>

class Planet : public PhysicObject, public Collidable {
private:
	double planetRadius; ///< Rayon de la planète; prise aux poles de celle-ci
	Sphere sphere; ///< Model de la sphère décrivant la planète
	std::list<DecorModel> landDecor; ///< Tous les composantes faisant partie du décor de la planète

public:

	/// \param radius Rayon de la planète
	/// \param material Matériau de la planète
	/// \param sphereColor Couleur de la sphère
	/// \param position Position de la planète dans le monde 3D
	Planet(double radius, const std::string& material, Vector3d sphereColor, Vector3d position) : PhysicObject(4 * PI * radius * radius * radius / 3, material), planetRadius(radius), sphere(radius, 0, true) {
		sphere.setColor(sphereColor.x, sphereColor.y, sphereColor.z);
		sphere.move(position);
	}

	/// \brief Ajoute un décor à la planète
	/// \param fileName Nom du fichier du décor
	/// \param textureID ID de la texture pour la planète
	/// \param hasHitbox True si le décor a une hitbox
	/// \param pos Position du décor
	/// \param scaleFactor Facteur de grandissement du modèle de décor
	DecorModel& addDecorModel(const char* fileName, const unsigned int& textureID, const Vector3d& pos, double scaleFactor) {
		landDecor.emplace_back(fileName, textureID);
		landDecor.back().move(pos);
		landDecor.back().scale(scaleFactor);

		return landDecor.back();
	}

	/// \brief Retourne le model de la planète
	/// \return Model de la planète
	Model& getModel() {
		return sphere.getModel();
	}

	/// \brief Retourne le rayon de la planète
	/// \param Rayon de la planète
	const double getPlanetRadius() {
		return planetRadius;
	}

	/// \brief Applique un facteur de grandissement sur le rayon
	/// \param factor Facteur de grandissement
	void scaleRadius(double factor){
		planetRadius *= factor;
	}

	/// \brief Applique une force sur un objet
	/// \param object Objet sur lequel la force est appliquée
	/// \return Vecteur de la force appliquée
	Vector3d getGravitationalForce(Piece& object) {
		Vector3d distance(sphere.getPos() - object.getPosition());
		double distanceNorm(distance.getNorm());
		return distance.normalize() * (G * volume * material.density * object.getTotalMass()) / (distanceNorm * distanceNorm);
	}

	/// \brief Retourne la position de la planète
	///< \return Vecteur position
	Vector3d getPosition() const {
		return sphere.getPos();
	}

	/// \brief Permet de savoir si un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	/// \param direction Vecteur de direction
	/// \param position Position du vecteur
	/// \return Point d'intersection le plus proche entre le vecteur et le mesh et le r minimal
	Intersect isColliding(const bool& infiniteLength, const Vector3d& direction, const Vector3d& position) const {
		return sphere.getAABB().intersects(infiniteLength, direction, position)
			   ? isCollidingVector(infiniteLength, sphere.getVertexCount(), &sphere.getVertices(), &sphere.getNormals(), direction, position)
			   : Intersect();
	}

	/// \brief Retourne le tableau de vertices du model de la planète
	/// \return Tableau de vertices du model la planète
	const double* getVertices() const {
		return &sphere.getVertices();
	}

	/// \brief Retourne le nombre de vertices de la planète
	/// \return Nombre de vertices de la planète
	const unsigned int& getVertexCount() const {
		return sphere.getVertexCount();
	}

	///< \brief Dessine la planète
	void draw() {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		sphere.draw();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

};

#endif
