/// \brief Permet d'obtenir une position relative aux Panel parents.

#ifndef SOURCE_POSITION_H
#define SOURCE_POSITION_H

class Position {
private:
	Position* parent;    ///< Position du Panel parent. AGIS COMME UNE RÉFÉRENCE

	friend class Panel;    ///< Permet à Panel d'ajouté sa Position comme parente.

public:
	int x;    ///< Composante x de la position locale.
	int y;    ///< Composante y de la position locale.

	/// \param x Valeur initiale de la composante x locale.
	/// \param y Valeur initiale de la composante y locale.
	Position(const int& x, const int& y) : parent(nullptr), x(x), y(y) {}

	/// \brief Obtention de la composante x globale.
	/// \return La composante x comme affichée sur l'écran.
	int getGlobalX() {
		return x + (parent ? parent->getGlobalX() : 0);
	}

	/// \brief Obtention de la composante y globale.
	/// \return La composante y comme affichée sur l'écran.
	int getGlobalY() {
		return y + (parent ? parent->getGlobalY() : 0);
	}

	/// \brief Affectation des composantes de position locale.
	/// \param x Valeur de la composante x locale.
	/// \param y Valeur de la composante y locale.
	void setLocalPos(const int& x, const int& y) {
		this->x = x;
		this->y = y;
	}

	/// \brief Affectation des composantes de position globale.
	/// \param x Valeur de la composante x globale.
	/// \param y Valeur de la composante y globale.
	void setGlobalPos(const int& x, const int& y) {
		this->x = x - (parent ? parent->getGlobalX() : 0);
		this->y = y - (parent ? parent->getGlobalY() : 0);
	}

	/// \brief Affectation de la composante x globale.
	/// \param x Valeur de la composante x globale.
	void setGlobalX(const int& x) {
		this->x = x - (parent ? parent->getGlobalX() : 0);
	}

	/// \brief Affectation de la composante y globale.
	/// \param x Valeur de la composante y globale.
	void setGlobalY(const int& y) {
		this->y = y - (parent ? parent->getGlobalY() : 0);
	}

	/// \brief Déplace la Position de dx en x et dy en y.
	/// \param dx Déplacement à effectuer en x.
	/// \param dy Déplacement à effectuer en y.
	void move(const int& dx, const int& dy) {
		x += dx;
		y += dy;
	}

	/// \brief Obtention de la Position parente.
	/// \return Pointeur vers la Position parente.
	Position& getParent() {
		return *parent;
	}

	/// \brief Détermine si le point {x, y} est dans le rectangle {globalX, globalY, w, h}
	/// \return Vrai si le point est dans le rectangle.
	bool containsPoint(const int& w, const int& h, const int& x, const int& y) {
		int gX(getGlobalX());
		int gY(getGlobalY());

		return (x >= gX && x < gX + w) && (y >= gY && y < gY + h);
	}

	/// \brief Détermine si le point {x, y} est dans le rectangle {globalX, globalY, w, h}
	/// \return Vrai si le point est dans le rectangle.
	bool containsPoint(const double& w, const double& h, double x, double y, const double& angle = 0., const std::pair<int, int>& pivot = {0, 0}) {
		double gX(getGlobalX());
		double gY(getGlobalY());

		if (angle) {
			double sinned(std::sin(angle));
			double cossed(std::cos(angle));
			double ny(sinned * (x - pivot.first - gX) + cossed * (y - pivot.second - gY) + pivot.second + gY);
			x = cossed * (x - pivot.first - gX) - sinned * (y - pivot.second - gY) + pivot.first + gX;
			y = ny;
		}

		return (x >= gX && x < gX + w) && (y >= gY && y < gY + h);
	}
};

#endif //SOURCE_POSITION_H
