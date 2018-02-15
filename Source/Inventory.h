#ifndef SOURCE_INVENTORY_H
#define SOURCE_INVENTORY_H

#include <list>
#include "Panel.h"
#include "Rect.h"
#include "Button.h"
#include "Label.h"
#include "PieceManager.h"
#include "GameData.h"
#include "BlueprintPiece.h"
#include "PlayerData.h"

/// \brief Panel représentant l'inventaire

class Inventory : public Panel {
private:
	struct PiecePanel : public Panel {
		std::string name;	///< Nom de la pièce
		unsigned count;	///< Nombre de pièces (PlayerData::inventory.second)
		Button background;	///< Bouton bleu en arrière-plan
		Label label;	///< Label contenant le nom de la pièce
		Label countLabel;	///< Label contenant le nombre de pièce
		std::list<BlueprintPiece*>& blueprints;	///< Référence à BuildScene::blueprints
		ModelGroup& anchorPoints;	///< Référence à BuildScene::anchorPoints
		ModelGroup& objects;	///< Référence à BuildScene::objects
		PiecePanel*& toRemove;	///< Référence à Inventory::toRemove

		/// \param elementIndex Position du PiecePanel dans l'inventaire, pour positionner en y
		PiecePanel(const std::string& name, const unsigned& count, const unsigned& elementIndex, std::list<BlueprintPiece*>& blueprints, ModelGroup& anchorPoints, ModelGroup& objects, PiecePanel*& toRemove)
				: Panel(0, List::MARGIN, List::MARGIN + elementIndex * (List::NODE_HEIGHT + List::SPACE_BETWEEN)),
				  name(name),
				  count(count),
				  background(SDLGLContext::loadTexture(("Textures2D/" + PieceManager::getPieceData(name).type + ".png").c_str()), SDLGLContext::loadTexture(("Textures2D/" + PieceManager::getPieceData(name).type + ".png").c_str()), 0, 0, 0, List::WIDTH - 2 * List::MARGIN, List::NODE_HEIGHT),
				  label(1, 0, 0, name, "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}),
				  countLabel(1, background.w, 0, "x" + std::to_string(count), "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}),
				  blueprints(blueprints),
				  anchorPoints(anchorPoints),
				  objects(objects),
				  toRemove(toRemove) {
			addComponent("background", &background);
			addComponent("label", &label);
			addComponent("countLabel", &countLabel);

			countLabel.pos.x -= countLabel.w;

			background.onClick = [this](VisualComponent2D& visComp) {
				if (this->count) {
					operator--();
					this->blueprints.push_back(new BlueprintPiece(PieceManager::getPieceData(this->name), this->anchorPoints));
					this->blueprints.back()->move(0, 2, 0, this->blueprints);
					this->objects.addModel(this->blueprints.back()->pieceName + std::to_string(this->blueprints.back()->getID()),this->blueprints.back()->getModel());
				}
			};
		}

		/// \brief Incrémente le nombre de pièce
		/// \return Référence au PiecePanel
		PiecePanel& operator++() {
			countLabel.setStr("x" + std::to_string(++count));
			countLabel.pos.x = background.w - countLabel.w;

			return *this;
		}

		/// \brief Décrémente le nombre de pièce et retire une pièce de PlayerData::inventory
		/// \return Référence au PiecePanel
		PiecePanel& operator--() {
			countLabel.setStr("x" + std::to_string(--count));
			countLabel.pos.x = background.w - countLabel.w;

			PlayerData::removeItem(name);

			if (!count)
				toRemove = this;

			return *this;
		}
	};

	std::list<PiecePanel> piecePanels;	///< Liste de PiecePanel
	PiecePanel* toRemove;	///< Pointeur vers un PiecePanel à retirer

public:
	Rect background;	///< Arrière-plan de l'inventaire

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de pos.x
	/// \param y Valeur initiale de pos.y
	/// \param blueprints Référence à BuildScene::blueprints
	/// \param blueprints Référence à BuildScene::anchorPoints
	/// \param blueprints Référence à BuildScene::objects
	Inventory(const int& priority, const int& x, const int& y, std::list<BlueprintPiece*>& blueprints, ModelGroup& anchorPoints, ModelGroup& objects)
			: Panel(priority, x, y),
			  background(0, 0, 0, List::WIDTH, 2 * List::MARGIN + List::NODE_HEIGHT + ((unsigned)PieceManager::getPiecesData().size() - 2) * (List::NODE_HEIGHT + List::SPACE_BETWEEN), true, Vector3d(.5, .5, .5)),
			  toRemove(nullptr) {
		addComponent("background", &background);

		EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, &background);

		for (const auto& piece : PlayerData::inventory)
			if (PieceManager::getPieceData(piece.first).type != "Cockpit")
				addPiecePanel(piece.first, piece.second, blueprints, anchorPoints, objects);
	}

	~Inventory() {
		EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, &background);
	}

	/// \brief Instruction à effectué à chaque tick.
	void update() {
		if (toRemove)
			for (std::list<PiecePanel>::iterator it = piecePanels.begin(); it != piecePanels.end() && toRemove; it++)
				if (&(*it) == toRemove) {
					removePanel(it->name);
					piecePanels.erase(it++);
					for (; it != piecePanels.end(); it++)
						it->pos.y -= List::NODE_HEIGHT + List::SPACE_BETWEEN;
					toRemove = nullptr;
				}

		Panel::update();
	}

	/// \return Vrai si le curseur est sur l'inventaire
	const bool& isRemoving() const {
		return background.isMouseOver();
	}

	/// \brief Ajoute un PiecePanel à l'inventaire
	/// \param name Nom de la pièce
	/// \param count Nombre de pièces
	/// \param blueprints Référence à BuildScene::blueprints
	/// \param blueprints Référence à BuildScene::anchorPoints
	/// \param blueprints Référence à BuildScene::objects
	void addPiecePanel(const std::string& name, const unsigned& count, std::list<BlueprintPiece*>& blueprints, ModelGroup& anchorPoints, ModelGroup& objects) {
		piecePanels.emplace_back(name, count, piecePanels.size(), blueprints, anchorPoints, objects, toRemove);
		addPanel(name, &piecePanels.back());
	}

	/// \brief Incrémente la pièce si le PiecePanel est présent
	/// \param name Nom de la pièce
	/// \return Vrai si piecePanels a un PiecePanel associé à name
	bool addPiece(const std::string& name) {
		std::list<PiecePanel>::iterator it = std::find_if(piecePanels.begin(), piecePanels.end(), [&name](const PiecePanel& piecePanel) {
			return piecePanel.name == name;
		});
		if (it == piecePanels.end())
			return false;
		++(*it);
		return true;
	}
};

#endif //SOURCE_INVENTORY_H
