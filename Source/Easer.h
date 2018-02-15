#ifndef SOURCE_EASER_H
#define SOURCE_EASER_H

/// \brief Permet un mouvement "ease in/out" cubique d'un vecteur de n'importe quel dimension

#include <array>
#include <functional>

template <unsigned char elementNum>	///> Nombre de composantes
class Easer {
private:
	std::array<double, elementNum> start;	///< Valeurs des éléments au début du mouvement
	std::array<double, elementNum> delta;	///< Incrément du mouvement pour chaque éléments
	unsigned currentTime;	///< Nombre de ticks depuis le début du mouvement
	unsigned duration;	///< Durée en ticks du mouvement

public:
	std::list<std::array<double*, elementNum>> elementList;	///> Liste de tableaux contenant les références aux composantes à modifier

	Easer() {
		delta.fill(0.);
	}

	/// \brief Mise à jour de Easer
	void update() {
		if (isMoving()) {
			if (currentTime == duration)
				delta.fill(0.);
			else {
				double time(currentTime++);

				time /= (double) duration / 2.;

				if (time < 1)
					for (unsigned i = 0; i < elementNum; i++)
						for (std::array<double*, elementNum>& elements : elementList)
							*elements[i] = start[i] + delta[i] / 2 * time * time * time;
				else {
					time -= 2;
					for (unsigned i = 0; i < elementNum; i++)
						for (std::array<double*, elementNum>& elements : elementList)
							*elements[i] = start[i] + delta[i] / 2 * (time * time * time + 2);
				}
			}
		}
	}

	/// \return Vrai s'il bouge
	bool isMoving() const {
		return delta[0];
	}

	/// \brief Assigne la cible et durée du mouvement
	/// \param duration Durée du mouvement de ticks (DELTATIME)
	/// \param target Vecteur cible du mouvement
	void setTarget(const unsigned& duration, const std::array<double, elementNum>& target) {
		for (unsigned i = 0; i < elementNum; i++)
			delta[i] = target[i] - (start[i] = *elementList.back()[i]);
		currentTime = 0;
		this->duration = duration;
	}
};
#endif //SOURCE_EASER_H
