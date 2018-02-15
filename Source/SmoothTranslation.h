#ifndef SOURCE_SMOOTHTRANSLATION_H
#define SOURCE_SMOOTHTRANSLATION_H

/// \brief Permet un mouvement accéléré d'un vecteur de n'importe quel dimension

#include <array>

template <unsigned char elementNum>	///> Nombre de composantes
struct SmoothTranslation {
	std::array<double, elementNum> elements;	///> Array contenant les composantes de vitesse
	double accel;	///> Facteur de la quantité ajoutée à la vitesse
	double friction;	///> Facteur multiplié à elements à chaque tick
	const bool factorBasedFriction;
	double min;	///> Vitesse minimale avant de devenir nulle
	double max;	///> Vitesse maximale

	SmoothTranslation(const double& accel, const double& friction, const double& min, const double& max, const bool& factorBasedFriction = true)
			: accel(accel),
			  friction(friction),
			  factorBasedFriction(factorBasedFriction),
			  min(min),
			  max(max) {
		elements.fill(0.);
	}

	/// \brief Applique la friction
	void update() {
		if (operator bool()) {
			if (getNorm() < min)
				elements.fill(0.);
			else if (factorBasedFriction)
				for (unsigned char i = 0; i < elementNum; i++)
					elements[i] *= friction;
			else
				for (unsigned char i = 0; i < elementNum; i++)
					elements[i] -= friction;
		}
	}

	/// \brief Ajoute les composantes de arr * accel à elements
	/// \param arr Array de composantes à ajouter
	void accelerate(std::array<double, elementNum> arr) {
		for (unsigned char i = 0; i < elementNum; i++)
			if (arr[i] < 0)
				elements[i] = std::max(elements[i] + arr[i] * accel, -max);
			else
				elements[i] = std::min(elements[i] + arr[i] * accel, max);
	}

	/// \brief Obtention de la norme du vecteur vitesse
	double getNorm() {
		if (elementNum == 1)
			return std::abs(elements[0]);

		double norm = 0.;
		for (double element : elements)
			norm += element * element;
		return std::sqrt(norm);
	}

	/// \brief Obtention d'une des composantes
	/// \param i Index de la compsante
	double operator[](unsigned char i) {
		return elements[i];
	}

	/// \brief Vrai si les composantes sont toutes à 0
	operator bool() {
		for (double element : elements)
			if (element)
				return true;
		return false;
	}
};

#endif //SOURCE_SMOOTHTRANSLATION_H
