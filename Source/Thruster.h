#ifndef SOURCE_THRUSTER_H
#define SOURCE_THRUSTER_H

#include "Piece.h"
#include "SmoothTranslation.h"
#include "FuelContainer.h"

/// \brief Propulseur sans essence

class Thruster : public Piece {
public:
	const double power; ///< Puissance du Thruster en temps réel

	Thruster(const PieceData& pieceData, Piece* mother, std::function<void(Piece*)>& addPiece, std::function<void(Piece*)>& removePiece)
			: Piece(pieceData, mother, addPiece, removePiece),
			  power(pieceData.data.at("power")) {
		functions.reserve(1);
		functions.emplace_back("Thruster" + std::to_string(id) + " : Thrust", [this](Piece& cockpit) {
			cockpit.addForce(power * model.getTransform().getDirection(), getPosition());
		});
	}
};

#endif //SOURCE_THRUSTER_H
