#ifndef SOURCE_ANCHORPOINT_H
#define SOURCE_ANCHORPOINT_H

#include "Model.h"
#include "Vector3d.h"
#include "GameData.h"
#include "Singleton.h"
#include "Camera.h"
#include "Sphere.h"

///\brief Lien entre les pièces
///
/// Classe représentant une sphère qui, à la collision d'un même objet, pourra lier 2 BlueprintPiece


enum class Socket : unsigned int {
	Universal = 0,
	Fuel = 1
};

class AnchorPoint : public Sphere {
private:
	Vector3d normal; ///< Normale du point d'ancrage

public:
	bool binded;    ///< True si un objet est déjà collé à ce point d'ancrage
	const bool isMale;    ///< Genre du point d'ancrage; si vrai->masculin
	const Socket socket;    ///< Type de prise pour le point d'ancrage

	/// \param anchorInfo Tuple contenant l'information de l'AnchorPoint
	AnchorPoint(const std::tuple<bool, unsigned int, Vector3d, Vector3d>& anchorInfo)
			: Sphere(ANCHORPOINT_RADIUS, 0, false), isMale(std::get<0>(anchorInfo)), socket(static_cast<Socket>(std::get<1>(anchorInfo))), normal(std::get<3>(anchorInfo)), binded(false) {
		move(std::get<2>(anchorInfo));

		if (isMale)
			setColor(0.0, 0.0, 1.0);
		else
			setColor(1.0, 0.2, 1.0);
	}

	/// \param isMale Genre du AnchorPoint
	/// \param socket Type de Socket du AnchorPoint
	/// \param position Position du centre
	/// \param normal Normale du point d'ancrage
	AnchorPoint(bool isMale, Socket socket, Vector3d pos, Vector3d normal)
			: Sphere(ANCHORPOINT_RADIUS, 0, false), isMale(isMale), socket(socket), normal(normal), binded(false) {
		move(pos);

		if (isMale)
			setColor(0.0, 0.0, 1.0);
		else
			setColor(1.0, 0.2, 1.0);
	}

	/// \brief Permet la rotation de l'AnchorPoint autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		Sphere::rotateAroundLine(axis, point, angle);
		Matrix4d().setRotationAroundLine(axis, point, angle).rotate(normal);
	}


	/// \brief Détecte si une collision a lieu avec un autre point d'ancrage
	/// \param anchorPoint Point d'ancrage avec qui on teste la collision
	/// \return Booléen true s'il y a collision
	bool isColliding(AnchorPoint& anchorPoint) {
		if (!binded && !anchorPoint.binded) {
			Vector3d PB(anchorPoint.getPos() - Singleton<Camera>::getInstance()->pos);
			Vector3d front = (Singleton<Camera>::getInstance()->target - Singleton<Camera>::getInstance()->pos).normalize();
			double r = Vector3d::dotProduct(front, getPos() - Singleton<Camera>::getInstance()->pos) / Vector3d::dotProduct(front, PB);

			if ((getPos() - (Singleton<Camera>::getInstance()->pos + r * PB)).getNorm() <= getRadius() * (r + 1)) {
				if (anchorPoint.isMale != isMale) {
					if (anchorPoint.socket == socket) {
						anchorPoint.setColor(0.0, 1.0, 0.0);
						setColor(0.0, 1.0, 0.0);
						return true;
					}
				}
				// Si les sockets sont incompatibles, la couleur apparaît en rouge
				anchorPoint.setColor(1.0, 0.0, 0.0);
				setColor(1.0, 0.0, 0.0);
				return true;
			}
		}

		if (anchorPoint.isMale)
			anchorPoint.setColor(0.0, 0.0, 1.0);
		else
			anchorPoint.setColor(1.0, 0.2, 1.0);

		if (isMale)
			setColor(0.0, 0.0, 1.0);
		else
			setColor(1.0, 0.2, 1.0);

		return false;
	}

	/// \brief Accès à normal
	/// \return normal
	Vector3d& getNormal() {
		return normal;
	}
};

#endif
