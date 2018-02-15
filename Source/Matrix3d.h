#ifndef SOURCE_MATRIX3D_H
#define SOURCE_MATRIX3D_H

/// \brief Matrice 3x3.
///
/// Principalement utilisée pour décrire une rotation.
///

#include <array>
#include <algorithm>
#include "Vector3d.h"

struct Matrix3d {
	std::array<double, 9> matrix;    ///< Éléments de la matrice 3x3, ligne par ligne

	/// \brief Affectation des éléments de la matrice
	Matrix3d& set(const std::array<double, 9>& array) {
		matrix = array;
	}

	/// \brief Défini la matrice identitaire.
	Matrix3d& setIdentity() {
		for (unsigned char i = 0; i < 9; i++)
			matrix[i] = !(i % 4);

		return *this;
	}

	/// \brief Affectation ligne par ligne des valeurs de la matrice
	/// \param r1 Vecteur correspondant à la ligne 1
	/// \param r2 Vecteur correspondant à la ligne 2
	/// \param r3 Vecteur correspondant à la ligne 3
	/// \return Référence à la matrice
	Matrix3d& setRows(const Vector3d& r1, const Vector3d& r2, const Vector3d& r3) {
		matrix[0] = r1.x;
		matrix[1] = r1.y;
		matrix[2] = r1.z;

		matrix[3] = r2.x;
		matrix[4] = r2.y;
		matrix[5] = r2.z;

		matrix[6] = r3.x;
		matrix[7] = r3.y;
		matrix[8] = r3.z;

		return *this;
	}


	/// \brief Affectation colonne par colonne des valeurs de la matrice
	/// \param r1 Vecteur correspondant à la colonne 1
	/// \param r2 Vecteur correspondant à la colonne 2
	/// \param r3 Vecteur correspondant à la colonne 3
	/// \return Référence à la matrice
	Matrix3d& setColumns(const Vector3d& c1, const Vector3d& c2, const Vector3d& c3) {
		matrix[0] = c1.x;
		matrix[1] = c2.x;
		matrix[2] = c3.x;

		matrix[3] = c1.y;
		matrix[4] = c2.y;
		matrix[5] = c3.y;

		matrix[6] = c1.z;
		matrix[7] = c2.z;
		matrix[8] = c3.z;

		return *this;
	}

	/// \brief Transpose la matrice
	/// \return Référence à la matrice
	Matrix3d& transpose() {
		std::swap(matrix[1], matrix[3]);
		std::swap(matrix[2], matrix[6]);
		std::swap(matrix[5], matrix[7]);

		return *this;
	}

	/// \brief Calcul le déterminant de la matrice par la méthode
	/// \return Le dé
	double getDeterminant() const {
		//Uses Sarrus's rule http://m-hikari.com/ija/ija-password-2009/ija-password5-8-2009/hajrizajIJA5-8-2009.pdf#page=3
		//Though that shouldn't affect computation

		return   matrix[0] * matrix[4] * matrix[8]
			   + matrix[1] * matrix[5] * matrix[6]
			   + matrix[2] * matrix[3] * matrix[7]
			   - matrix[2] * matrix[4] * matrix[6]
			   - matrix[0] * matrix[5] * matrix[7]
			   - matrix[1] * matrix[3] * matrix[8];
	}

	/// \brief Effectue une rotation autour de l'axe X
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix3d& xRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return *this *= Matrix3d().setColumns(Vector3d::RIGHT, {0., cossed, sinned}, {0., -sinned, cossed});
	}

	/// \brief Effectue une rotation autour de l'axe Y
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix3d& yRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return *this *= Matrix3d().setColumns({cossed, 0., -sinned}, Vector3d::UP, {sinned, 0., cossed});
	}

	/// \brief Effectue une rotation autour de l'axe Z
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix3d& zRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return *this *= Matrix3d().setColumns({cossed, sinned, 0.}, {-sinned, cossed, 0.}, Vector3d::FRONT);
	}

	/// \brief Effectue une multiplication à un vecteur.
	/// \param vec Vecteur à multiplier.
	/// \return Vecteur affecté par la multiplication.
	Vector3d& multiply(Vector3d& vec) const {
		return vec.set(matrix[0] * vec.x + matrix[1] * vec.y + matrix[2] * vec.z,
					   matrix[3] * vec.x + matrix[4] * vec.y + matrix[5] * vec.z,
					   matrix[6] * vec.x + matrix[7] * vec.y + matrix[8] * vec.z);
	}

	/// \brief Assignation à l'addition de la matrice à une autre
	/// \param mat Deuxième matrice.
	/// \return Référence à la matrice.
	Matrix3d& operator+=(const Matrix3d& mat) {
		for (unsigned char i = 0; i < 9; i++)
			matrix[i] += mat.matrix[i];

		return *this;
	}

	/// \brief Assignation à la soustraction de la matrice à une autre
	/// \param mat Deuxième matrice.
	/// \return Référence à la matrice.
	Matrix3d& operator-=(const Matrix3d& mat) {
		for (unsigned char i = 0; i < 9; i++)
			matrix[i] -= mat.matrix[i];

		return *this;
	}

	/// \brief Assignation à la multiplication de la matrice à une autre
	/// \param mat Deuxième matrice.
	/// \return Référence à la matrice.
	Matrix3d& operator*=(const Matrix3d& mat) {
		return set({
						   matrix[0] * mat.matrix[0] + matrix[1] * mat.matrix[3] + matrix[2] * mat.matrix[6], matrix[0] * mat.matrix[1] + matrix[1] * mat.matrix[4] + matrix[2] * mat.matrix[7], matrix[0] * mat.matrix[2] + matrix[1] * mat.matrix[5] + matrix[2] * mat.matrix[8],
						   matrix[3] * mat.matrix[0] + matrix[4] * mat.matrix[3] + matrix[5] * mat.matrix[6], matrix[3] * mat.matrix[1] + matrix[4] * mat.matrix[4] + matrix[5] * mat.matrix[7], matrix[3] * mat.matrix[2] + matrix[4] * mat.matrix[5] + matrix[5] * mat.matrix[8],
						   matrix[6] * mat.matrix[0] + matrix[7] * mat.matrix[3] + matrix[8] * mat.matrix[6], matrix[6] * mat.matrix[1] + matrix[7] * mat.matrix[4] + matrix[8] * mat.matrix[7], matrix[6] * mat.matrix[2] + matrix[7] * mat.matrix[5] + matrix[8] * mat.matrix[8]
				   });
	}

	///brief Mets en string le contenu de la string
	///return Retourne la matrice dans une std::string
	std::string toString() const {
		std::string str;

		for (unsigned char i = 0; i < 9; i += 3)
			str += std::to_string(matrix[i]) + "\t" + std::to_string(matrix[i + 1]) + "\t" + std::to_string(matrix[i + 2]) + "\n";

		return str;
	}
};

/// \brief Addition de deux matrices.
/// \param v1 Première matrice.
/// \param v2 Deuxième matrice.
/// \return Matrice résultante.
Matrix3d operator+(const Matrix3d& m1, const Matrix3d& m2) {
	return Matrix3d().set({
								  m1.matrix[0] + m2.matrix[0],
								  m1.matrix[1] + m2.matrix[1],
								  m1.matrix[2] + m2.matrix[2],

								  m1.matrix[3] + m2.matrix[3],
								  m1.matrix[4] + m2.matrix[4],
								  m1.matrix[5] + m2.matrix[5],

								  m1.matrix[6] + m2.matrix[6],
								  m1.matrix[7] + m2.matrix[7],
								  m1.matrix[8] + m2.matrix[8]
						  });
}

/// \brief Soustraction de deux matrices.
/// \param v1 Première matrice.
/// \param v2 Deuxième matrice.
/// \return Matrice résultante.
Matrix3d operator-(const Matrix3d& m1, const Matrix3d& m2) {
	return Matrix3d().set({
								  m1.matrix[0] - m2.matrix[0],
								  m1.matrix[1] - m2.matrix[1],
								  m1.matrix[2] - m2.matrix[2],

								  m1.matrix[3] - m2.matrix[3],
								  m1.matrix[4] - m2.matrix[4],
								  m1.matrix[5] - m2.matrix[5],

								  m1.matrix[6] - m2.matrix[6],
								  m1.matrix[7] - m2.matrix[7],
								  m1.matrix[8] - m2.matrix[8]
						  });
}

/// \brief Multiplication de deux matrices
/// \param m1 Première matrice.
/// \param m2 Deuxième matrice.
/// \return Matrice résultante.
Matrix3d operator*(const Matrix3d& m1, const Matrix3d& m2) {
	return Matrix3d().set({
								  m1.matrix[0] * m2.matrix[0] + m1.matrix[1] * m2.matrix[3] + m1.matrix[2] * m2.matrix[6], m1.matrix[0] * m2.matrix[1] + m1.matrix[1] * m2.matrix[4] + m1.matrix[2] * m2.matrix[7], m1.matrix[0] * m2.matrix[2] + m1.matrix[1] * m2.matrix[5] + m1.matrix[2] * m2.matrix[8],
								  m1.matrix[3] * m2.matrix[0] + m1.matrix[4] * m2.matrix[3] + m1.matrix[5] * m2.matrix[6], m1.matrix[3] * m2.matrix[1] + m1.matrix[4] * m2.matrix[4] + m1.matrix[5] * m2.matrix[7], m1.matrix[3] * m2.matrix[2] + m1.matrix[4] * m2.matrix[5] + m1.matrix[5] * m2.matrix[8],
								  m1.matrix[6] * m2.matrix[0] + m1.matrix[7] * m2.matrix[3] + m1.matrix[8] * m2.matrix[6], m1.matrix[6] * m2.matrix[1] + m1.matrix[7] * m2.matrix[4] + m1.matrix[8] * m2.matrix[7], m1.matrix[6] * m2.matrix[2] + m1.matrix[7] * m2.matrix[5] + m1.matrix[8] * m2.matrix[8]
						  });
}

/// \brief Multiplication d'un vecteur 3d à une matrice 3x3
/// \param mat Matrice.
/// \param vec Vecteur.
/// \return Vecteur résultant.
Vector3d operator*(const Matrix3d& mat, const Vector3d& vec) {
	return {
			mat.matrix[0] * vec.x + mat.matrix[1] * vec.y + mat.matrix[2] * vec.z,
			mat.matrix[3] * vec.x + mat.matrix[4] * vec.y + mat.matrix[5] * vec.z,
			mat.matrix[6] * vec.x + mat.matrix[7] * vec.y + mat.matrix[8] * vec.z
	};
}

/// \brief Multiplication d'une matrice à un scalaire
/// \param mat Matrice.
/// \param scalar Scalaire.
/// \return Matrice résultante.
Matrix3d operator*(const double& scalar, const Matrix3d& mat) {
	return Matrix3d().set({
								  scalar * mat.matrix[0],
								  scalar * mat.matrix[1],
								  scalar * mat.matrix[2],
								  scalar * mat.matrix[3],
								  scalar * mat.matrix[4],
								  scalar * mat.matrix[5],
								  scalar * mat.matrix[6],
								  scalar * mat.matrix[7],
								  scalar * mat.matrix[8]
						  });
}

/// \brief Multiplication d'une matrice à un scalaire
/// \param mat Matrice.
/// \param scalar Scalaire.
/// \return Matrice résultante.
Matrix3d operator*(const Matrix3d& mat, const double& scalar) {
	return scalar * mat;
}

#endif //SOURCE_MATRIX3D_H
