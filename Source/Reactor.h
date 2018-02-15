/// \brief Classe de la pièce réacteur.
///
/// Classe conteant toutes les propriétés du réacteur.
///

#ifndef SOURCE_REACTOR_H
#define SOURCE_REACTOR_H

#include "Piece.h"
#include "SmoothTranslation.h"
#include "FuelContainer.h"
#include "Flame.h"

class Reactor : public Piece {
protected:
	/// \brief Déplace la flame du réacteur de movement, puis lance Piece::moveChild
	/// \param movement Déplacement
	void moveChild(const Vector3d& movement) {
		flame.transform.translate(movement);

		Piece::moveChild(movement);
	}

public:
	const double ejectionSpeed; ///< Vitesse d'éjection du carburant (m/s)
	const double ejectedGas; ///< Quantité de gas éjecté par seconde (kg/s)
	double reactorPower; ///< Puissance du réacteur activé en temps réel
	Flame& flame; ///< Flamme créée par le réacteur

	Reactor(const PieceData& pieceData, Piece* mother, std::function<void(Piece*)>& addPiece, std::function<void(Piece*)>& removePiece, Flame& flame)
			: Piece(pieceData, mother, addPiece, removePiece),
			  ejectionSpeed(pieceData.data.at("ejectionSpeed")),
			  ejectedGas(pieceData.data.at("ejectedGas")),
			  flame(flame),
			  reactorPower(0.) {
		functions.reserve(2);
		functions.emplace_back("Reactor #" + std::to_string(id) + " : Heat Up", [this](Piece& cockpit) {
			if (static_cast<FuelContainer*>(this->mother)->hasFuel()) {
				reactorPower = std::fmin(1., reactorPower + .015);

				PlayerData::reactorPower = (unsigned)std::round(this->reactorPower * 100.);
			}
		});
		functions.emplace_back("Reactor #" + std::to_string(id) + " : Cool Down", [this](Piece& cockpit) {
			reactorPower = std::fmax(0., reactorPower - .015);

			PlayerData::reactorPower = (unsigned)std::round(this->reactorPower * 100.);
		});

		flame.applyTransform(Matrix4d().setXRotation(PI));
	}

	/// \brief Mets à jour reactorForce, et les enfants à chaque tick
	void updateMother() {
		if (static_cast<FuelContainer*>(mother)->hasFuel()) {
			if (reactorPower > 0.) {
				static_cast<FuelContainer*>(mother)->consumption(ejectedGas);
				getUltimateMother()->addForce(reactorPower * ejectionSpeed * ejectedGas * model.getTransform().getDirection(), getPosition());
			}
		}else
			functions[1].second(*getUltimateMother());

		if (flame.on = reactorPower > VERY_SMALL_DOUBLE) {
			flame.setStrength(reactorPower / 3);
			flame.length = reactorPower;
			flame.update();
		}

		Piece::updateMother();
	}

	/// \brief Déplace la pièce et ses enfants par le vecteur movement
	void move(const Vector3d& movement) {
		flame.transform.translate(movement);

		Piece::move(movement);
	}

	/// \brief Permet la rotation de la pièce autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		flame.transform = Matrix4d().setRotationAroundLine(axis, point, angle) * flame.transform;

		Piece::rotateAroundLine(axis, point, angle);
	}
};

#endif //SOURCE_REACTOR_H
