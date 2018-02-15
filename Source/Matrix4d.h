#ifndef SDL2OPENGL_MATRIX4D_H
#define SDL2OPENGL_MATRIX4D_H

/// \brief Matrice 4x4.
///
/// Classe représentant une matrice de 16 composantes réelles.
///

#include <array>
#include "Vector3d.h"
#include "Matrix3d.h"

struct Matrix4d {
	std::array<double, 16> matrix;    ///< Éléments de la matrice 4x4, ligne par ligne

	Matrix4d() = default;

	Matrix4d(const Matrix4d& mat) = default;

	Matrix4d(Matrix4d&& mat) = default;

	Matrix4d& operator=(const Matrix4d& mat) = default;

	Matrix4d& operator=(Matrix4d&& mat) = default;

	/// \brief Affectation des éléments de la matrice
	Matrix4d& set(const std::array<double, 16>& array) {
		matrix = array;

		return *this;
	}

	/// \brief Affectation des éléments aux éléments d'une matrice 3d
	Matrix4d& set(const Matrix3d& mat) {
		matrix = {
				mat.matrix[0], mat.matrix[1], mat.matrix[2], 0.,
				mat.matrix[3], mat.matrix[4], mat.matrix[5], 0.,
				mat.matrix[6], mat.matrix[7], mat.matrix[8], 0.,
				0., 0., 0., 1.
		};

		return *this;
	}

	/// \brief Défini la matrice identitaire.
	Matrix4d& setIdentity() {
		for (unsigned char i = 0; i < 16; i++)
			matrix[i] = !(i % 5);

		return *this;
	}

	/// \brief Défini la matrice de projection d'openGL.
	/// \param left indice de gauche
	/// \param right indice de droite
	/// \param top indice du haut
	/// \param bottom indice du bas
	/// \param nearPlane distance entre l'observateur et le plan rapproché
	/// \param farPlane distance entre l'observateur et le plan éloigné
	void setProjection(const double& left, const double& right, const double& top, const double& bottom, const double& nearPlane, const double& farPlane) {
		if (left != right && top != bottom && nearPlane != farPlane) {
			matrix = {
					(2 * nearPlane) / (right - left), 0., (right + left) / (right - left), 0.,
					0., (2 * nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.,
					0., 0., -(farPlane + nearPlane) / (farPlane - nearPlane), (-2 * farPlane * nearPlane) / (farPlane - nearPlane),
					0., 0., -1., 0.
			};
		}
	}

	/// \brief Défini la matrice de vue d'openGL.
	/// \param side vecteur vers le côté de l'observateur
	/// \param up vecteur vers le haut de l'observateur
	/// \param front vecteur vers le devant de l'observateur
	void setView(const Vector3d& side, const Vector3d& up, const Vector3d& front) {
		matrix = {
				side.x, side.y, side.z, 0.,
				up.x, up.y, up.z, 0.,
				-front.x, -front.y, -front.z, 0.,
				0., 0., 0., 1.
		};
	}

	/// \return Un pointeur tableau des éléments de la matrice colonne par colonne
	double* getColumnMajorData() const {
		return new double[16]{
				matrix[0],
				matrix[4],
				matrix[8],
				matrix[12],

				matrix[1],
				matrix[5],
				matrix[9],
				matrix[13],

				matrix[2],
				matrix[6],
				matrix[10],
				matrix[14],

				matrix[3],
				matrix[7],
				matrix[11],
				matrix[15]
		};
	}

	/// \brief Calcul une translation à partir d'un vecteur
	/// \param translation Vecteur de translation
	/// \return Référence à la matrice.
	Matrix4d& translate(const Vector3d& translation) {
		matrix[3] += translation.x;
		matrix[7] += translation.y;
		matrix[11] += translation.z;

		return *this;
	}

	/// \brief Calcul une translation à partir des composantes
	/// \param x Composante x
	/// \param y Composante y
	/// \param z Composante z
	/// \return Référence à la matrice.
	Matrix4d& translate(const double& x, const double& y, const double& z) {
		matrix[3] += x;
		matrix[7] += y;
		matrix[11] += z;

		return *this;
	}

	/// \return Vecteur position
	Vector3d getPos() const {
		return {matrix[3], matrix[7], matrix[11]};
	}

	/// \return Vecteur unitaire direction, ou rotate(Vector3d::UP)
	Vector3d getDirection() const {
		return Vector3d(matrix[1], matrix[5], matrix[9]).normalize();
	}

	/// \return La trace de la matrice
	double getTrace() const {
		return matrix[0] + matrix[5] + matrix[10] + matrix[15];
	}


	Vector3d getVectorPointAxis(const Vector3d& point) const {
		Vector3d direction(getDirection());
		Vector3d pointPos(getPos() - point);
		return pointPos + direction * (direction * -pointPos);
	}

	/// \brief Effectue une rotation autour de l'axe X
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix4d& xRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return *this *= Matrix4d().setXRotation(angle);
	}

	/// \brief Effectue une rotation autour de l'axe Y
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix4d& yRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return *this *= Matrix4d().setYRotation(angle);
	}

	/// \brief Effectue une rotation autour de l'axe Z
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix4d& zRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return *this *= Matrix4d().setZRotation(angle);
	}

	/// \brief Défini une matrice de rotation autour d'une ligne
	/// \param axis Vecteur de direction correspondant à la ligne
	/// \param point Vecteur d'un point sur la ligne
	/// \param angle Angle de rotation en radian
	Matrix4d& setRotationAroundLine(Vector3d axis, const Vector3d& point, const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));
		double iCossed(1 - cossed);
		axis.normalize();

		return set({
						   axis.x * axis.x + (axis.y * axis.y + axis.z * axis.z) * cossed,
						   axis.x * axis.y * iCossed - axis.z * sinned,
						   axis.x * axis.z * iCossed + axis.y * sinned,
						   (point.x * (axis.y * axis.y + axis.z * axis.z) - axis.x * (point.y * axis.y + point.z * axis.z)) * iCossed + (point.y * axis.z - point.z * axis.y) * sinned,

						   axis.x * axis.y * iCossed + axis.z * sinned,
						   axis.y * axis.y + (axis.x * axis.x + axis.z * axis.z) * cossed,
						   axis.y * axis.z * iCossed - axis.x * sinned,
						   (point.y * (axis.x * axis.x + axis.z * axis.z) - axis.y * (point.x * axis.x + point.z * axis.z)) * iCossed + (point.z * axis.x - point.x * axis.z) * sinned,

						   axis.x * axis.z * iCossed - axis.y * sinned,
						   axis.y * axis.z * iCossed + axis.x * sinned,
						   axis.z * axis.z + (axis.x * axis.x + axis.y * axis.y) * cossed,
						   (point.z * (axis.x * axis.x + axis.y * axis.y) - axis.z * (point.x * axis.x + point.y * axis.y)) * iCossed + (point.x * axis.y - point.y * axis.x) * sinned,

						   0., 0., 0., 1.
				   });
	}

	/// \brief Défini une matrice de rotation autour de l'axe X
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix4d& setXRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return set({
						   1., 0., 0., 0.,
						   0., cossed, -sinned, 0.,
						   0., sinned, cossed, 0.,
						   0., 0., 0., 1.
				   });
	}

	/// \brief Défini une matrice de rotation autour de l'axe Y
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix4d& setYRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return set({
						   cossed, 0., sinned, 0.,
						   0., 1., 0., 0.,
						   -sinned, 0., cossed, 0.,
						   0., 0., 0., 1.
				   });
	}

	/// \brief Défini une matrice de rotation autour de l'axe Z
	/// \param angle Angle de rotation en radian
	/// \return Référence à la matrice
	Matrix4d& setZRotation(const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));

		return set({
						   cossed, -sinned, 0., 0.,
						   sinned, cossed, 0., 0.,
						   0., 0., 1., 0.,
						   0., 0., 0., 1.
				   });
	}

	/// \brief Effectue une multiplication à un vecteur.
	/// \param vec Vecteur à multiplier.
	/// \return Vecteur affecté par la multiplication.
	Vector3d& multiply(Vector3d& vec) const {
		vec.set(matrix[0] * vec.x + matrix[1] * vec.y + matrix[2] * vec.z + matrix[3],
				matrix[4] * vec.x + matrix[5] * vec.y + matrix[6] * vec.z + matrix[7],
				matrix[8] * vec.x + matrix[9] * vec.y + matrix[10] * vec.z + matrix[11]);
		return vec;
	}

	/// \brief Effectue une multiplication à un vecteur sans la translation.
	/// \param vec Vecteur à multiplier.
	/// \return Vecteur affecté par la multiplication.
	Vector3d& rotate(Vector3d& vec) const {
		vec.set(matrix[0] * vec.x + matrix[1] * vec.y + matrix[2] * vec.z,
				matrix[4] * vec.x + matrix[5] * vec.y + matrix[6] * vec.z,
				matrix[8] * vec.x + matrix[9] * vec.y + matrix[10] * vec.z);
		return vec;
	}

	/// \brief Assignation à l'addition de la matrice à une autre
	/// \param mat Deuxième matrice.
	/// \return Référence à la matrice.
	Matrix4d& operator+=(const Matrix4d& mat) {
		for (unsigned char i = 0; i < 16; i++)
			matrix[i] += mat.matrix[i];

		return *this;
	}

	/// \brief Assignation à la soustraction de la matrice à une autre
	/// \param mat Deuxième matrice.
	/// \return Référence à la matrice.
	Matrix4d& operator-=(const Matrix4d& mat) {
		for (unsigned char i = 0; i < 16; i++)
			matrix[i] -= mat.matrix[i];

		return *this;
	}

	/// \brief Assignation à la multiplication de la matrice à une autre
	/// \param mat Deuxième matrice.
	/// \return Référence à la matrice.
	Matrix4d& operator*=(const Matrix4d& mat) {
		return set({
						   matrix[0] * mat.matrix[0] + matrix[1] * mat.matrix[4] + matrix[2] * mat.matrix[8] + matrix[3] * mat.matrix[12], matrix[0] * mat.matrix[1] + matrix[1] * mat.matrix[5] + matrix[2] * mat.matrix[9] + matrix[3] * mat.matrix[13], matrix[0] * mat.matrix[2] + matrix[1] * mat.matrix[6] + matrix[2] * mat.matrix[10] + matrix[3] * mat.matrix[14], matrix[0] * mat.matrix[3] + matrix[1] * mat.matrix[7] + matrix[2] * mat.matrix[11] + matrix[3] * mat.matrix[15],
						   matrix[4] * mat.matrix[0] + matrix[5] * mat.matrix[4] + matrix[6] * mat.matrix[8] + matrix[7] * mat.matrix[12], matrix[4] * mat.matrix[1] + matrix[5] * mat.matrix[5] + matrix[6] * mat.matrix[9] + matrix[7] * mat.matrix[13], matrix[4] * mat.matrix[2] + matrix[5] * mat.matrix[6] + matrix[6] * mat.matrix[10] + matrix[7] * mat.matrix[14], matrix[4] * mat.matrix[3] + matrix[5] * mat.matrix[7] + matrix[6] * mat.matrix[11] + matrix[7] * mat.matrix[15],
						   matrix[8] * mat.matrix[0] + matrix[9] * mat.matrix[4] + matrix[10] * mat.matrix[8] + matrix[11] * mat.matrix[12], matrix[8] * mat.matrix[1] + matrix[9] * mat.matrix[5] + matrix[10] * mat.matrix[9] + matrix[11] * mat.matrix[13], matrix[8] * mat.matrix[2] + matrix[9] * mat.matrix[6] + matrix[10] * mat.matrix[10] + matrix[11] * mat.matrix[14], matrix[8] * mat.matrix[3] + matrix[9] * mat.matrix[7] + matrix[10] * mat.matrix[11] + matrix[11] * mat.matrix[15],
						   matrix[12] * mat.matrix[0] + matrix[13] * mat.matrix[4] + matrix[14] * mat.matrix[8] + matrix[15] * mat.matrix[12], matrix[12] * mat.matrix[1] + matrix[13] * mat.matrix[5] + matrix[14] * mat.matrix[9] + matrix[15] * mat.matrix[13], matrix[12] * mat.matrix[2] + matrix[13] * mat.matrix[6] + matrix[14] * mat.matrix[10] + matrix[15] * mat.matrix[14], matrix[12] * mat.matrix[3] + matrix[13] * mat.matrix[7] + matrix[14] * mat.matrix[11] + matrix[15] * mat.matrix[15]
				   });
	}

	/// \brief Converti la matrice en texte.
	/// \return La string contenant le texte.
	std::string toString() {
		std::string str;

		for (unsigned char i = 0; i < 16; i += 4)
			str += std::to_string(matrix[i]) + "\t" + std::to_string(matrix[i + 1]) + "\t" + std::to_string(matrix[i + 2]) + "\t" + std::to_string(matrix[i + 3]) + "\n";

		return str;
	}
};

/// \brief Addition de deux matrices.
/// \param v1 Première matrice.
/// \param v2 Deuxième matrice.
/// \return Matrice résultante.
Matrix4d operator+(const Matrix4d& m1, const Matrix4d& m2) {
	return Matrix4d().set({
								  m1.matrix[0] + m2.matrix[0],
								  m1.matrix[1] + m2.matrix[1],
								  m1.matrix[2] + m2.matrix[2],
								  m1.matrix[3] + m2.matrix[3],

								  m1.matrix[4] + m2.matrix[4],
								  m1.matrix[5] + m2.matrix[5],
								  m1.matrix[6] + m2.matrix[6],
								  m1.matrix[7] + m2.matrix[7],

								  m1.matrix[8] + m2.matrix[8],
								  m1.matrix[9] + m2.matrix[9],
								  m1.matrix[10] + m2.matrix[10],
								  m1.matrix[11] + m2.matrix[11],

								  m1.matrix[12] + m2.matrix[12],
								  m1.matrix[13] + m2.matrix[13],
								  m1.matrix[14] + m2.matrix[14],
								  m1.matrix[15] + m2.matrix[15]
						  });
}

/// \brief Soustraction de deux matrices.
/// \param v1 Première matrice.
/// \param v2 Deuxième matrice.
/// \return Matrice résultante.
Matrix4d operator-(const Matrix4d& m1, const Matrix4d& m2) {
	return Matrix4d().set({
								  m1.matrix[0] - m2.matrix[0],
								  m1.matrix[1] - m2.matrix[1],
								  m1.matrix[2] - m2.matrix[2],
								  m1.matrix[3] - m2.matrix[3],

								  m1.matrix[4] - m2.matrix[4],
								  m1.matrix[5] - m2.matrix[5],
								  m1.matrix[6] - m2.matrix[6],
								  m1.matrix[7] - m2.matrix[7],

								  m1.matrix[8] - m2.matrix[8],
								  m1.matrix[9] - m2.matrix[9],
								  m1.matrix[10] - m2.matrix[10],
								  m1.matrix[11] - m2.matrix[11],

								  m1.matrix[12] - m2.matrix[12],
								  m1.matrix[13] - m2.matrix[13],
								  m1.matrix[14] - m2.matrix[14],
								  m1.matrix[15] - m2.matrix[15]
						  });
}

/// \brief Multiplication de deux matrices
/// \param m1 Première matrice.
/// \param m2 Deuxième matrice.
/// \return Matrice résultante.
Matrix4d operator*(const Matrix4d& m1, const Matrix4d& m2) {
	return Matrix4d().set({
								  m1.matrix[0] * m2.matrix[0] + m1.matrix[1] * m2.matrix[4] + m1.matrix[2] * m2.matrix[8] + m1.matrix[3] * m2.matrix[12],
								  m1.matrix[0] * m2.matrix[1] + m1.matrix[1] * m2.matrix[5] + m1.matrix[2] * m2.matrix[9] + m1.matrix[3] * m2.matrix[13],
								  m1.matrix[0] * m2.matrix[2] + m1.matrix[1] * m2.matrix[6] + m1.matrix[2] * m2.matrix[10] + m1.matrix[3] * m2.matrix[14],
								  m1.matrix[0] * m2.matrix[3] + m1.matrix[1] * m2.matrix[7] + m1.matrix[2] * m2.matrix[11] + m1.matrix[3] * m2.matrix[15],

								  m1.matrix[4] * m2.matrix[0] + m1.matrix[5] * m2.matrix[4] + m1.matrix[6] * m2.matrix[8] + m1.matrix[7] * m2.matrix[12],
								  m1.matrix[4] * m2.matrix[1] + m1.matrix[5] * m2.matrix[5] + m1.matrix[6] * m2.matrix[9] + m1.matrix[7] * m2.matrix[13],
								  m1.matrix[4] * m2.matrix[2] + m1.matrix[5] * m2.matrix[6] + m1.matrix[6] * m2.matrix[10] + m1.matrix[7] * m2.matrix[14],
								  m1.matrix[4] * m2.matrix[3] + m1.matrix[5] * m2.matrix[7] + m1.matrix[6] * m2.matrix[11] + m1.matrix[7] * m2.matrix[15],

								  m1.matrix[8] * m2.matrix[0] + m1.matrix[9] * m2.matrix[4] + m1.matrix[10] * m2.matrix[8] + m1.matrix[11] * m2.matrix[12],
								  m1.matrix[8] * m2.matrix[1] + m1.matrix[9] * m2.matrix[5] + m1.matrix[10] * m2.matrix[9] + m1.matrix[11] * m2.matrix[13],
								  m1.matrix[8] * m2.matrix[2] + m1.matrix[9] * m2.matrix[6] + m1.matrix[10] * m2.matrix[10] + m1.matrix[11] * m2.matrix[14],
								  m1.matrix[8] * m2.matrix[3] + m1.matrix[9] * m2.matrix[7] + m1.matrix[10] * m2.matrix[11] + m1.matrix[11] * m2.matrix[15],

								  m1.matrix[12] * m2.matrix[0] + m1.matrix[13] * m2.matrix[4] + m1.matrix[14] * m2.matrix[8] + m1.matrix[15] * m2.matrix[12],
								  m1.matrix[12] * m2.matrix[1] + m1.matrix[13] * m2.matrix[5] + m1.matrix[14] * m2.matrix[9] + m1.matrix[15] * m2.matrix[13],
								  m1.matrix[12] * m2.matrix[2] + m1.matrix[13] * m2.matrix[6] + m1.matrix[14] * m2.matrix[10] + m1.matrix[15] * m2.matrix[14],
								  m1.matrix[12] * m2.matrix[3] + m1.matrix[13] * m2.matrix[7] + m1.matrix[14] * m2.matrix[11] + m1.matrix[15] * m2.matrix[15]
						  });
}

/// \brief Multiplication d'un vecteur 3d à une matrice 4x4
/// \param mat Matrice.
/// \param vec Vecteur.
/// \return Vecteur résultant.
Vector3d operator*(const Matrix4d& mat, const Vector3d& vec) {
	return {
			mat.matrix[0] * vec.x + mat.matrix[1] * vec.y + mat.matrix[2] * vec.z,
			mat.matrix[4] * vec.x + mat.matrix[5] * vec.y + mat.matrix[6] * vec.z,
			mat.matrix[8] * vec.x + mat.matrix[9] * vec.y + mat.matrix[10] * vec.z
	};
}

/// \brief Multiplication d'une matrice à un scalaire
/// \param mat Matrice.
/// \param scalar Scalaire.
/// \return Matrice résultante.
Matrix4d operator*(const double& scalar, const Matrix4d& mat) {
	return Matrix4d().set({
								  scalar * mat.matrix[0],
								  scalar * mat.matrix[1],
								  scalar * mat.matrix[2],
								  scalar * mat.matrix[3],
								  scalar * mat.matrix[4],
								  scalar * mat.matrix[5],
								  scalar * mat.matrix[6],
								  scalar * mat.matrix[7],
								  scalar * mat.matrix[8],
								  scalar * mat.matrix[9],
								  scalar * mat.matrix[10],
								  scalar * mat.matrix[11],
								  scalar * mat.matrix[12],
								  scalar * mat.matrix[13],
								  scalar * mat.matrix[14],
								  scalar * mat.matrix[15],
						  });
}

/// \brief Multiplication d'une matrice à un scalaire
/// \param mat Matrice.
/// \param scalar Scalaire.
/// \return Matrice résultante.
Matrix4d operator*(const Matrix4d& mat, const double& scalar) {
	return scalar * mat;
}

#endif //SDL2OPENGL_MATRIX4D_H
