/// \brief Interface donnant des propriétés physique à un objet

#ifndef SOURCE_PHYSICOBJECT_H
#define SOURCE_PHYSICOBJECT_H

#include <stack>
#include <map>
#include "Material.h"
#include "MaterialManager.h"
#include "Vector3d.h"
#include "Material.h"
#include "MaterialManager.h"

#define FRICTION 0.005

class PhysicObject {
protected:
	const double volume; ///< Volume de l'objet (m^3) déterminé à l'aide de Blender
	const Material& material; ///< Matériau de l'objet

	std::stack<Vector3d> forces;	///< Forces instantannée appliquées à l'objet
	std::stack<Vector3d> torques; ///< Moments de force instantannés sur l'objet

    Vector3d centerOfMass; ///< Centre de masse global de l'objet
    double angularMass; ///< Moment d'inertie de l'objet
	double totalMass; ///< Masse totale

	/*
	 *  aluminium : 2 700 kg/m^3
	 *  Terre : 5 515 kg/m^3
	 *  Mars : 3 934 kg/m^3
	 *  Lune : 3 346 kg/m^3
	 *
	 * */

	///\return Moment de force résultant
	Vector3d getTotalTorques(){
		Vector3d totalTorque;

		while (!torques.empty()) {
			totalTorque += torques.top();
			torques.pop();
		}

		return totalTorque;
	}

	/// \return Force résultante
	Vector3d getTotalForces() {
		Vector3d totalForce;

		while (!forces.empty()) {
			totalForce += forces.top();
			forces.pop();
		}

		return totalForce;
	}

public:
	Vector3d speed, ///< Déplacement au prochain frame
			angularSpeed; ///< Déplacement angulaire au prochain frame

	/// \param volume Valeur initiale du volume
	/// \param material Materiel de l'objet
	PhysicObject(const double& volume, const std::string& material)
			: volume(volume), material(MaterialManager::getMaterial(material)), centerOfMass(Vector3d::ZERO) {}

	/// \brief Mets à jour la vitesse à chaque tick
	virtual void update(const double& levelRatio) {
        speed *= 1. - levelRatio * FRICTION;
		speed += (getTotalForces() / totalMass) * DELTATIME;
        angularSpeed *= 1. - std::max(levelRatio, 0.1) * FRICTION;
        angularSpeed += (getTotalTorques() / angularMass) * DELTATIME;
	}

	/// \brief Ajoute une force
	/// \param force Vecteur de force en Newton
	void addForce(const Vector3d& force, const Vector3d& applicationPoint) {
		forces.push(force);
        torques.push(force / (centerOfMass - applicationPoint));
	}

	///\brief Obtention de la densité de l'objet
	///\return La densité (kg/m^3)
	const double& getDensity() const {
		return material.density;
	}

	/// \brief Masse de la pièce locale
	double getLocalMass() const {
		return volume * material.density;
	}

	///\brief Obtention du volume de l'objet
	///\return Le volume (m^3)
	const double& getVolume() const {
		return volume;
	}

	///\brief Obtention de la position de l'objet
	///\return Le vecteur position
	virtual Vector3d getPosition() const = 0;
};

#endif //SOURCE_PHYSICOBJECT_H
