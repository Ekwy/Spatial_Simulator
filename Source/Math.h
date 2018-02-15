/// \brief Ressources mathématiques
///
/// Classe contenant diverses fonctions mathématiques utiles.
///

#ifndef SOURCE_MATH_H
#define SOURCE_MATH_H

#include <cmath>
#include <limits>
#include "Vector3d.h"
#include "Matrix3d.h"
#include "Matrix4d.h"

#define VERY_SMALL_DOUBLE 0.000'000'000'000'001
#define SMALL_DOUBLE 0.000'000'001
const double PI = std::acos(-1);

long double operator "" _deg(long double degree) {
	return degree / 180.0 * PI;
}

class Math {
public:
	/// \brief Convertir un radian en degré.
	/// \param rad Angle en radians à convertir en degré.
	/// \return L'angle en degré.
	static double radToDeg(double rad) {
		return rad / PI * 180.0;
	}

	/// \brief Convertir un degré en radian.
	/// \param rad Angle en degré à convertir en radians.
	/// \return L'angle en radians.
	static double degToRad(double deg) {
		return deg / 180.0 * PI;
	}

	/// \brief Trouver l'angle entre 2 vecteurs.
	/// \param vec1 Premier vecteur.
	/// \param vec2 Second vecteur.
	/// \return L'angle entre les 2 vecteurs.
	static double findAngle(const Vector3d& v1, const Vector3d& v2) {
		return std::acos((v1 * v2) / (v1.getNorm() * v2.getNorm()));
	}

	/// \brief Calcul la distance entre un point et une ligne
	/// \param point Position du point
	/// \param line Vecteur de direction de la ligne
	/// \param pointOnLine Position d'un point sur la ligne
	/// \return Distance entre le point et la ligne
	static double distancePointLine(const Vector3d& point, const Vector3d& line, const Vector3d& pointOnLine) {
		Vector3d direction = Vector3d(line).normalize();
		Vector3d pointToPoint(pointOnLine - point);
		return (pointToPoint - direction * (pointToPoint * direction)).getNorm();
	}

	/// \brief Produit un vecteur orientation correspondant à un vecteur direction
	/// \param direction Vecteur direction
	/// \return Vecteur orientation (pitch, yaw, roll) en radian
	static Vector3d directionToOrientation(const Vector3d& direction) {
		return {std::atan2(direction.y, direction.z),
				std::atan2(direction.z, direction.x),
				0.};
	}

	/// \brief Permet de faire une rotation autour d'un axe
	/// \param angle Angle de rotation en radian
	/// \param axis Axe de rotation
	/// \param vec Point à faire tourner autour de l'axe
	/// \param target Point origine de l'axe de rotation
	static void rotateAroundLine(const double& angle, Vector3d axis, const Vector3d& pivot, Vector3d& vec) {
		double sinned = std::sin(angle);
		double cossed = std::cos(angle);

		axis.normalize();

		double vx = vec.x;
		double vy = vec.y;
		double vz = vec.z;

		vec.x = (pivot.x * (axis.y * axis.y + axis.z * axis.z) - axis.x * (pivot.y * axis.y + pivot.z * axis.z - axis.x * vx - axis.y * vy - axis.x * vy)) * (1 - cossed) + vx * cossed + (-pivot.z * axis.y + pivot.y * axis.z - axis.z * vy + axis.y * vz) * sinned;
		vec.y = (pivot.y * (axis.x * axis.x + axis.z * axis.z) - axis.y * (pivot.x * axis.x + pivot.z * axis.z - axis.x * vx - axis.y * vy - axis.x * vy)) * (1 - cossed) + vy * cossed + (pivot.z * axis.x - pivot.x * axis.z + axis.z * vx - axis.x * vz) * sinned;
		vec.z = (pivot.z * (axis.x * axis.x + axis.y * axis.y) - axis.z * (pivot.x * axis.x + pivot.y * axis.y - axis.x * vx - axis.y * vy - axis.x * vy)) * (1 - cossed) + vz * cossed + (-pivot.y * axis.x + pivot.x * axis.y - axis.y * vx + axis.x * vy) * sinned;
	}

	/// \brief Permet de faire une rotation autour d'un axe
	/// \param angle Angle de rotation en radian
	/// \param axis Axe de rotation
	/// \param vecs Points à faire tourner autour de l'axe
	/// \param vertexCount Nombre de points à faire tourner
	/// \param pivot Point origine de l'axe de rotation
	static void rotateAroundLine(const double& angle, Vector3d axis, const Vector3d& pivot, double* vecs, const unsigned int& vertexCount) {
		double sinned = std::sin(angle);
		double cossed = std::cos(angle);

		axis.normalize();

		double vx;
		double vy;
		double vz;

		for (unsigned int i = 0; i < vertexCount * 3; i += 3) {
			vx = vecs[i];
			vy = vecs[i + 1];
			vz = vecs[i + 2];

			vecs[i] = (pivot.x * (axis.y * axis.y + axis.z * axis.z) - axis.x * (pivot.y * axis.y + pivot.z * axis.z - axis.x * vx - axis.y * vy - axis.z * vz)) * (1 - cossed) + vx * cossed + (-pivot.z * axis.y + pivot.y * axis.z - axis.z * vy + axis.y * vz) * sinned;
			vecs[i + 1] = (pivot.y * (axis.x * axis.x + axis.z * axis.z) - axis.y * (pivot.x * axis.x + pivot.z * axis.z - axis.x * vx - axis.y * vy - axis.z * vz)) * (1 - cossed) + vy * cossed + (pivot.z * axis.x - pivot.x * axis.z + axis.z * vx - axis.x * vz) * sinned;
			vecs[i + 2] = (pivot.z * (axis.x * axis.x + axis.y * axis.y) - axis.z * (pivot.x * axis.x + pivot.y * axis.y - axis.x * vx - axis.y * vy - axis.z * vz)) * (1 - cossed) + vz * cossed + (-pivot.y * axis.x + pivot.x * axis.y - axis.y * vx + axis.x * vy) * sinned;
		}
	}

	/// \brief Produit une matrice de rotation à partir d'une rotation autour d'un axe
	/// \param angle Angle de rotation en radian
	/// \param axis Axe de rotation
	/// \return Vecteur orientation
	static Matrix3d getAngleAxisRotation(Vector3d axis, const double& angle) {
		double sinned(std::sin(angle));
		double cossed(std::cos(angle));
		double iCossed(1 - cossed);
		axis.normalize();

		return Matrix3d().set({
									  cossed + axis.x * axis.x * iCossed, axis.x * axis.y * iCossed - axis.z * sinned, axis.x * axis.z * iCossed + axis.y * sinned,
									  axis.y * axis.x * iCossed + axis.z * sinned, cossed + axis.y * axis.y * iCossed, axis.y * axis.z * iCossed - axis.x * sinned,
									  axis.z * axis.x * iCossed - axis.y * sinned, axis.z * axis.y * iCossed + axis.x * sinned, cossed + axis.z * axis.z * iCossed
							  });
	}

	/// \brief Définie la matrice de rotation d'un vecteur à un autre
	/// \param a Premier vecteur
	/// \param b Deuxième vecteur
	/// \return Matrice de rotation
	static Matrix3d rotationBetween(Vector3d a, Vector3d b) {    //https://math.stackexchange.com/a/476311
		Matrix3d mat;
		a.normalize();
		b.normalize();

		if (a == b)
			return mat.setIdentity();
		else if (a == -b) {
			mat.set({1., 0., 0., 0., -1., 0., 0., 0., -1.});
		}else {
			Vector3d v(a / b);
			double s(v.getNorm());
			double c(a * b);

			Matrix3d vx;
			vx.set({0., -v.z, v.y, v.z, 0., -v.x, -v.y, v.x, 0.});

			mat.setIdentity() += vx + (vx * vx) * (1 / (1 + c));
		}

		return mat;
	}

	/// \brief Produit une matrice 4x4 qui inverse la rotation de la matrice donnée
	/// \param mat Matrice de rotation
	/// \return Matrice de rotation inverse
	static Matrix4d getInverseRotation(const Matrix4d& mat) {
		if (mat.matrix[1] < VERY_SMALL_DOUBLE && mat.matrix[2] < VERY_SMALL_DOUBLE && mat.matrix[4] < VERY_SMALL_DOUBLE && mat.matrix[6] < VERY_SMALL_DOUBLE && mat.matrix[8] < VERY_SMALL_DOUBLE && mat.matrix[9] < VERY_SMALL_DOUBLE)
			return Matrix4d().setIdentity();

		Vector3d axis(mat.matrix[9] - mat.matrix[6],
					  mat.matrix[2] - mat.matrix[8],
					  mat.matrix[4] - mat.matrix[1]);

		Vector3d perp(axis.getPerpendicular());

		Vector3d rotatedPerp(perp);
		mat.rotate(rotatedPerp);

		double angle(-Vector3d::angleBetween(perp, rotatedPerp));

		return Matrix4d().setRotationAroundLine(axis, mat.getPos(), angle);
	}

	/// \brief Retourne vrai si les valeurs sont approximativement égales
	/// \param a Première valeur
	/// \param b Deuxième valeur
	/// \param tolerance Différence minimale pour être approximativement égales
	static bool areNearlyEqual(const double& a, const double& b, const double& tolerance) {
		return std::fabs(a - b) <= tolerance;
	}

	/// \brief Retourne vrai si les valeurs sont approximativement égales
	/// \param a Première valeur
	/// \param b Deuxième valeur
	static bool areNearlyEqual(const double& a, const double& b) {
		return std::fabs(a - b) < std::numeric_limits<double>::epsilon() * std::fmax(std::abs(a), std::abs(b));
	}

	/// \brief Retourne vrai si a est approximativement plus grande ou égale à b
	/// \param a Première valeur
	/// \param b Deuxième valeur
	static bool isNearlyGreaterOrEqual(const double& a, const double& b, const double& tolerance = std::numeric_limits<double>::epsilon()) {
		return a > b || areNearlyEqual(a, b, tolerance);
	}

	/// \brief Retourne vrai si a est approximativement plus petite ou égale à b
	/// \param a Première valeur
	/// \param b Deuxième valeur
	static bool isNearlyLessOrEqual(const double& a, const double& b, const double& tolerance = std::numeric_limits<double>::epsilon()) {
		return a < b || areNearlyEqual(a, b, tolerance);
	}

	/// \brief Retourne num si num != 0., sinon retourne elseNum
	static const double& notZero(const double& num, const double& elseNum = 1.) {
		return !areNearlyEqual(num, 0., VERY_SMALL_DOUBLE) ? num : elseNum;
	}
};

#endif
