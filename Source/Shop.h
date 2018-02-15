/// \brief Panel représentant le magasin
///
/// Classe permettant la création d'un magasin en listant tous les blueprints créés.
///

#ifndef SOURCE_SHOP_H
#define SOURCE_SHOP_H

#include "Panel.h"
#include "Rect.h"
#include "Button.h"
#include "Label.h"
#include "PieceManager.h"
#include "GameData.h"
#include "Piece.h"
#include "ModelGroup.h"

class Shop : public Panel {
private:
	Label cost; ///< Label avec l'argent disponible
	Inventory& inventory; ///< Référence à l'inventaire

	struct PiecePanel : public Panel {
		std::string name; ///< Nom de la pièce
		unsigned price; ///< Prix, il est en unsigned int pour éviter d'avoir les décimales lors de la conversion to_string(double)
		Button background; ///< Bouton bleu en arrière-plan
		Label label; ///< Label contenant le nom de la pièce
		Label labelPrice; ///< Label contenant le prix de la piece
		std::list<BlueprintPiece*>& pieces; ///< Référence à BuildScene::blueprints
		ModelGroup& anchorPoints; ///< Référence à BuildScene::anchorPoints
		ModelGroup& objects; ///< Référence à BuildScene::objects
		Inventory& inventory; ///< Référence à Inventory

		// TODO : Positionner les items relativement au panel...
		PiecePanel(const std::string& name, const unsigned& elementIndex, std::list<BlueprintPiece*>& pieces, ModelGroup& anchorPoints, ModelGroup& objects, Inventory& inventory)
				: Panel(0, List::MARGIN, List::MARGIN + elementIndex * (List::NODE_HEIGHT + List::SPACE_BETWEEN)),
				  name(name),
				  price(PieceManager::getPieceData(name).price),
				  background(SDLGLContext::loadTexture(("Textures2D/" + PieceManager::getPieceData(name).type + ".png").c_str()), SDLGLContext::loadTexture(("Textures2D/" + PieceManager::getPieceData(name).type + ".png").c_str()), 0, 0, 0, List::WIDTH - 2 * List::MARGIN, List::NODE_HEIGHT),
				  label(1, 0, 0, name, "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}),
				  labelPrice(1, List::WIDTH - 73, 0, std::to_string(price) + "k$", "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}),
				  pieces(pieces),
				  anchorPoints(anchorPoints),
				  objects(objects),
				  inventory(inventory) {

			addComponent("background", &background);
			addComponent("label", &label);
			addComponent("labelPrice", &labelPrice);

			background.onClick = [&name, &inventory, &pieces, &anchorPoints, &objects](VisualComponent2D& visComp) {
				if (PlayerData::money - PieceManager::getPieceData(name).price >= 0) {
					PlayerData::money -= PieceManager::getPieceData(name).price;
					PlayerData::addItem(name);

					if (!inventory.addPiece(name))
						inventory.addPiecePanel(name, 1, pieces, anchorPoints, objects);
				}
			};

			/*background.onMouseOver = [&pieceInfo, &name] (VisualComponent2D& visComp) {
				pieceInfo.setStr(PieceManager::getPieceData(name).type);
			};

			background.onMouseAway = [] (VisualComponent2D& visComp) {

			};*/
		}
	};

	std::list<PiecePanel> piecePanels;  ///< Liste de PiecePanel

public:
	Rect background; ///< Arrière-plan du magasin

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de pos.x
	/// \param y Valeur initiale de pos.y
	/// \param blueprints Référence à BuildScene::blueprints
	/// \param blueprints Référence à BuildScene::anchorPoints
	/// \param blueprints Référence à BuildScene::objects
	Shop(const int& priority, const int& x, const int& y, std::list<BlueprintPiece*>& pieces, ModelGroup& anchorPoints, ModelGroup& objects, Inventory& inventory)
			: Panel(priority, x, y),
			  inventory(inventory),
			  background(0, 0, 0, List::WIDTH, 2 * List::MARGIN + List::NODE_HEIGHT + ((unsigned)PieceManager::getPiecesData().size()) * (List::NODE_HEIGHT + List::SPACE_BETWEEN), false, Vector3d(.5, .5, .5)),
			  cost(1, List::MARGIN, List::MARGIN + PieceManager::getPiecesData().size() * (List::NODE_HEIGHT + List::SPACE_BETWEEN), "Solde : " + std::to_string(PlayerData::money) + "$", "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 204, 0, 255}) {

		addComponent("background", &background);
		addComponent("cost", &cost);

		for (const auto& pieceData : PieceManager::getPiecesData())
			if (pieceData.second.type != "Cockpit") {
				PiecePanel(pieceData.first, (unsigned)piecePanels.size(), pieces, anchorPoints, objects, inventory);
				piecePanels.emplace_back(pieceData.first, piecePanels.size(), pieces, anchorPoints, objects, inventory);
				addPanel(pieceData.first, &piecePanels.back());
			}
	}

	/// \brief Instruction à effectué à chaque tick.
	void update() {
		cost.setStr("Solde : " + std::to_string(PlayerData::money) + "k$");
	}
};

#endif //SOURCE_SHOP_H
