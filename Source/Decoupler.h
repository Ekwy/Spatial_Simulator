/// \brief classe des pièces decoupler
///
/// un découpleur permet de détacher des pièces de pa fusée
///

#include "Piece.h"

#ifndef SOURCE_DECOUPLER_H
#define SOURCE_DECOUPLER_H

class Decoupler : public Piece {
public:
	Decoupler(const PieceData& pieceData, Piece* mother, std::function<void(Piece*)>& addPiece, std::function<void(Piece*)>& removePiece) : Piece(pieceData, mother, addPiece, removePiece) {
		functions.reserve(1);
		functions.emplace_back("Decoupler" + std::to_string(id) + "::detach()", [this](Piece& cockpit) {
			detachFromMother();
			speed -= model.getTransform().getDirection();
			//ultimateMother->speed += model.getTransform().getDirection(); //FIXME Do we use this?
			});
	}
};

#endif
