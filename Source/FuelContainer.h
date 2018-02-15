/// \brief Classe de la pièce Fuelcontainer.
///
/// Classe conteant toutes les propriétés du fuelcontainer.
///

#ifndef SOURCE_FUELCONTAINER_H
#define SOURCE_FUELCONTAINER_H

#include "Piece.h"

class FuelContainer : public Piece {
private:
	double fuelLevel; ///< Quantité d'essence actuellement dans le conteneur.

public:
	const double capacity; ///< Capacité du conteneur en litres (TIP: Volume du model)

	FuelContainer(const PieceData& pieceData, Piece* mother, std::function<void(Piece*)>& addPiece,  std::function<void(Piece*)>& removePiece)
			: Piece(pieceData, mother, addPiece, removePiece),
			  capacity(pieceData.data.at("capacity")),
			  fuelLevel(capacity) {
        fuelLevel = capacity;
    }

	/// \brief Modifie le fuelLevel selon le gas ejecté
    void consumption(const double& ejectedGas){
        if((fuelLevel - (ejectedGas / 300.)) > 0.) {
            fuelLevel -= (ejectedGas / 300.);
			PlayerData::fuelLevel = (fuelLevel / capacity) * 100;
        }
    }

	/// \return Retourne vrai si le réservoir à essence n'est pas vide
    bool hasFuel() {
        return fuelLevel > 1.;
    }
};

#endif //SOURCE_FUELCONTAINER_H
