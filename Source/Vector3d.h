/// \brief Vecteur à 3 composantes.
///
/// Classe représentant un vecteur de trois composantes réels.
///

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

struct Vector3d {
	static const Vector3d ZERO; ///< Vecteur {0,0,0}
	static const Vector3d ONE; ///< Vecteur {1,1,1}
	static const Vector3d RIGHT;///< Vecteur {1,0,0}
	static const Vector3d UP;///< Vecteur {0,1,0}
	static const Vector3d FRONT;///< Vecteur {0,0,1}

	double x, ///< Composante x.
			y, ///< Composante y.
			z; ///< Composante z.

	/// \param x Valeur initiale de la composante x.
	/// \param y Valeur initiale de la composante y.
	/// \param z Valeur initiale de la composante z.
	Vector3d(const double& x, const double& y, const double& z) : x(x), y(y), z(z) {}

	Vector3d() : x(0.), y(0.), z(0.) {}

	/// \param v Vecteur à copier.
	Vector3d(const Vector3d& vec) = default;

	/// \brief Constructeur de mouvement
	Vector3d(Vector3d&& vec) = default;

	/// \brief Opérateur de copie
	Vector3d& operator=(const Vector3d& vec) = default;

	/// \brief Opérateur de mouvement
	Vector3d& operator=(Vector3d&& vec) = default;

	/// \brief Affectation des composantes du vecteur.
	/// \param x Valeur de la composante x.
	/// \param y Valeur de la composante y.
	/// \param z Valeur de la composante z.
	/// \return Référence au vecteur affecté.
	Vector3d& set(const double& x, const double& y, const double& z) {
		this->x = x;
		this->y = y;
		this->z = z;

		return *this;
	}

	/// \brief Normalisation du vecteur.
	/// \return Référence au vecteur normalisé.
	Vector3d& normalize() {
		double norm;

		if (norm = std::sqrt(x * x + y * y + z * z)) {
			x /= norm;
			y /= norm;
			z /= norm;
		}

		return *this;
	}

	/// \brief Obtention de la norme du vecteur.
	/// \return Norme du vecteur.
	double getNorm() const {
		return sqrt(x * x + y * y + z * z);
	}

	/// \brief Produit un vecteur perpendiculaire
	Vector3d getPerpendicular() const {
		double norm = std::sqrt(x * x + y * y + z * z);

		if (x / norm != 1.)
			return Vector3d(0., z / norm, -y / norm).normalize();    //return crossProduct(this, Vector::RIGHT)
		else
			return Vector3d(-z / norm, 0., x / norm).normalize();    //return crossProduct(this, Vector::UP)
	}

	/// \brief Retourne faux si le vecteur est nul.
	/// \return Faux si le vecteur est nul.
	operator bool() const {
		return getNorm() > std::numeric_limits<double>::epsilon();
	}

	/// \brief Inverse le signe des composantes
	/// \return Nouveau vecteur aux composantes inversés.
	Vector3d operator-() const {
		return Vector3d(-x, -y, -z);
	}

	/// \brief Assignation à l'addition de deux vecteurs
	/// \param scalar Vecteur à additionner.
	/// \return Référence au vecteur.
	Vector3d& operator+=(const Vector3d& v) {
		return set(x + v.x, y + v.y, z + v.z);
	}

	/// \brief Assignation à l'addition du vecteur à un scalaire
	/// \param scalar Valeur du scalaire.
	/// \return Référence au vecteur.
	Vector3d& operator+=(double scalar) {
		return set(x + scalar, y + scalar, z + scalar);
	}

	/// \brief Assignation à la soustraction de deux vecteurs
	/// \param scalar Vecteur à soustraction.
	/// \return Référence au vecteur.
	Vector3d& operator-=(const Vector3d& v) {
		return set(x - v.x, y - v.y, z - v.z);
	}

	/// \brief Assignation à la soustraction du vecteur à un scalaire
	/// \param scalar Valeur du scalaire.
	/// \return Référence au vecteur.
	Vector3d& operator-=(double scalar) {
		return set(x - scalar, y - scalar, z - scalar);
	}

	/// \brief Assignation à la multiplication du vecteur à un scalaire
	/// \param scalar Valeur du scalaire.
	/// \return Référence au vecteur.
	Vector3d& operator*=(double scalar) {
		return set(x * scalar, y * scalar, z * scalar);
	}

	/// \brief Assignation à la division du vecteur à un scalaire
	/// \param scalar Valeur du scalaire.
	/// \return Référence au vecteur.
	Vector3d& operator/=(double scalar) {
		return set(x / scalar, y / scalar, z / scalar);
	}

	/// \return La composante i du vecteur
	double& operator[](const unsigned char& i) {
		try {
			if (i > 2)
				throw std::out_of_range("Vector3d[unsigned char]");
		}catch(std::out_of_range e) {
			std::cerr << e.what() << ": " << "You tried to access the " << i + 1 << "th item of a 3d vector." << std::endl;
			std::exit(1);
		}

		return !i ? x : (i == 1 ? y : z);
	}

	/// \return Une référence constante à la composante i du vecteur
	const double& get(const unsigned char& i) const {
		try {
			if (i > 2)
				throw std::out_of_range("Vector3d[unsigned char]");
		}catch(std::out_of_range e) {
			std::cerr << e.what() << ": " << "You tried to access the " << i + 1 << "th item of a 3d vector." << std::endl;
			std::exit(1);
		}

		return !i ? x : (i == 1 ? y : z);
	}

	/// \brief Composition d'une chaîne de caractères représentant les composantes.
	/// \return Chaîne de caractères représentant les composantes.
	std::string toString() const {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	/// \brief Calcul l'angle entre deux vecteurs
	/// \return L'angle en radian
	static double angleBetween(const Vector3d& v1, const Vector3d& v2) {
		return std::acos(Vector3d::dotProduct(v1, v2) / (v1.getNorm() * v2.getNorm()));    //FIXME Using v1*v2 goes through operator bool()
	}

	/// \brief Produit scalaire.
	/// \param v1 Premier vecteur.
	/// \param v2 Second vecteur.
	/// \return Nouveau vecteur résultant.
	static double dotProduct(const Vector3d& v1, const Vector3d& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
};

const Vector3d Vector3d::ZERO = {0., 0., 0.};
const Vector3d Vector3d::ONE = {1., 1., 1.};
const Vector3d Vector3d::RIGHT = {1., 0., 0.};
const Vector3d Vector3d::UP = {0., 1., 0.};
const Vector3d Vector3d::FRONT = {0., 0., 1.};

/// \brief Assignation à == de 2 vecteurs
/// \param v Vecteur
/// \return Booléen
bool operator==(const Vector3d& v1, const Vector3d& v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

/// \brief Assignation à != de 2 vecteurs
/// \param v Vecteur
/// \return Booléen
bool operator!=(const Vector3d& v1, const Vector3d& v2) {
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

/// \brief Addition vectorielle.
/// \param v1 Premier vecteur.
/// \param v2 Second vecteur.
/// \return Nouveau vecteur résultant.
Vector3d operator+(const Vector3d& v1, const Vector3d& v2) {
	return Vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/// \brief Addition à un scalaire.
/// \param v Vecteur à additionner.
/// \param scalar Valeur du scalaire.
/// \return Nouveau vecteur résultant.
Vector3d operator+(const Vector3d& v, double scalar) {
	return Vector3d(v.x + scalar, v.y + scalar, v.z + scalar);
}

/// \brief Addition à un scalaire.
/// \param scalar Valeur du scalaire.
/// \param v Vecteur à additionner.
/// \return Nouveau vecteur résultant.
Vector3d operator+(double scalar, const Vector3d& v) {
	return v + scalar;
}

/// \brief Soustraction vectorielle.
/// \param v1 Premier vecteur.
/// \param v2 Second vecteur.
/// \return Nouveau vecteur résultant.
Vector3d operator-(const Vector3d& v1, const Vector3d& v2) {
	return Vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/// \brief Soustraction par un scalaire.
/// \param v Vecteur à soustraire.
/// \param scalar Valeur du scalaire.
/// \return Nouveau vecteur résultant.
Vector3d operator-(const Vector3d& v, double scalar) {
	return Vector3d(v.x - scalar, v.y - scalar, v.z - scalar);
}

/// \brief Produit scalaire.
/// \param v1 Premier vecteur.
/// \param v2 Second vecteur.
/// \return Nouveau vecteur résultant.
double operator*(const Vector3d& v1, const Vector3d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// \brief Produit par un scalaire.
/// \param v Vecteur à multiplier.
/// \param scalar Valeur du scalaire.
/// \return Nouveau vecteur résultant.
Vector3d operator*(const Vector3d& v, double scalar) {
	return Vector3d(v.x * scalar, v.y * scalar, v.z * scalar);
}

/// \brief Produit par un scalaire.
/// \param scalar Valeur du scalaire.
/// \param v Vecteur à multiplier.
/// \return Nouveau vecteur résultant.
Vector3d operator*(double scalar, const Vector3d& v) {
	return v * scalar;
}

/// \brief Produit vectoriel.
/// \param v1 Premier vecteur.
/// \param v2 Second vecteur.
/// \return Nouveau vecteur résultant.
Vector3d operator/(const Vector3d& v1, const Vector3d& v2) {
	return Vector3d(v1.y * v2.z - v2.y * v1.z, v2.x * v1.z - v1.x * v2.z, v1.x * v2.y - v2.x * v1.y);
}

/// \brief Quotient par un scalaire.
/// \param v Vecteur à multiplier.
/// \param scalar Valeur du scalaire.
/// \return Nouveau vecteur résultant.
Vector3d operator/(const Vector3d& v, double scalar) {
	return Vector3d(v.x / scalar, v.y / scalar, v.z / scalar);
}

#endif //VECTOR3D_H
