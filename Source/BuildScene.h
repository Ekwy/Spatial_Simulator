/// \brief Scène représentant la construction d'une fusée.
///
/// Classe Affichant tous les models liés à la construction d'une fusée.
///

#ifndef SOURCE_BUILDSCENE_H
#define SOURCE_BUILDSCENE_H

#include <SDL2/SDL_events.h>
#include <functional>
#include <vector>
#include "Scene.h"
#include "BlueprintPiece.h"
#include "EventDispatcher.h"
#include "GameData.h"
#include "Atmosphere.h"
#include "PieceManager.h"
#include "DecorModel.h"
#include "Button.h"
#include "CheckBox.h"
#include "Panel.h"
#include "PlayerData.h"
#include "Inventory.h"
#include "Shop.h"
#include "IntEaser.h"

class BuildScene : public Scene {
private:
	BlueprintPiece* selected; ///< Pièce sélectionnée
	double offset; ///< Distance entre la sélection et le curseur
	ModelGroup objects; ///< Objets à affichage proche
	ModelGroup farObjects; ///< Objets à affichage loin
	ModelGroup anchorPoints; ///< Tous les AnchorPoints
	DecorModel grassland; ///< Décor de pelouse
	DecorModel pipeline; ///< Décor de tuyau
	DecorModel platform; ///< Décor de plateforme de construction
	DecorModel launchTower; ///< Décor de tour de soutien pour la fusée
	std::vector<std::reference_wrapper<DecorModel>> decorModels; ///< Décor à afficher
	Atmosphere atmosphere;///< Atmosphère de la scène

	CheckBox missionSelection; ///< Bouton pour ouvrir le menu des missions
	CheckBox inventoring; ///< Bouton pour ouvrir l'inventaire
	CheckBox shopping; ///< Bouton pour ouvrir le menu de magasin
	Panel missionSelectionPanel;///< Panneau contenant l'ensemble des missions
	Button liftoffMission;///< Bouton de démarrage de la mission LiftOff

	Inventory inventory; ///< Inventaire du joueur
	Shop shop; ///< Magasin
	IntEaser<1> inventoryEaser; ///< IntEaser contrôllant le mouvement de inventory
	IntEaser<1> inventoringEaser; ///< IntEaser contrôllant le mouvement de inventoring
	IntEaser<1> shopEaser; ///< IntEaser contrôllant le mouvement de shop
	IntEaser<1> shoppingEaser; ///< IntEaser contrôllant le mouvement de shopping

	/// \brief Exécute les instructions pendant la mise à jour à chaque tick
	void periUpdate() {
		Scene::periUpdate();

		inventoryEaser.update();
		inventoringEaser.update();

		shopEaser.update();
		shoppingEaser.update();
	}

public:
	std::list<BlueprintPiece*> blueprints; ///< Liste de l'ensemble des mères racines présentes dans la BuildScene

	BuildScene() : Scene(),
				   grassland("Textures3D/land.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/grass_texture.png")),
				   pipeline("Textures3D/BuildScene/pipeline.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/pipeline_texture.png")),
				   platform("Textures3D/BuildScene/plateform.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/plateform_texture.png")),
				   launchTower("Textures3D/BuildScene/LaunchTowerNicolas.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/LaunchTower_texture.png")),
				   atmosphere("Textures3D/BuildScene/skybox_blue.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/spacebox.png"), 6'000.0, 1.0, 500., 2'000.),
				   missionSelection(SDLGLContext::loadTexture("Textures2D/MissionSelectionUnchecked.png"), SDLGLContext::loadTexture("Textures2D/MissionSelectionChecked.png"), 0, false, 0, 0, 350, 50),
				   missionSelectionPanel(0, 0, 50),
				   liftoffMission(SDLGLContext::loadTexture("Textures2D/LiftoffUnpressed.png"), SDLGLContext::loadTexture("Textures2D/LiftoffPressed.png"), 1, 25, 10, 300, 100),
				   selected(nullptr),
				   inventory(0, 10, WINDOW_HEIGHT, blueprints, anchorPoints, objects),
				   shop(0, WINDOW_WIDTH - (List::WIDTH + 10), WINDOW_HEIGHT, blueprints, anchorPoints, objects, inventory),
                   shopping(SDLGLContext::loadTexture("Textures2D/Shopping.png"), SDLGLContext::loadTexture("Textures2D/ShoppingSelected.png"), 0, false, WINDOW_WIDTH - 150 , WINDOW_HEIGHT - 100, 150, 100),
				   inventoring(SDLGLContext::loadTexture("Textures2D/BuildScene/Inventoring.png"), SDLGLContext::loadTexture("Textures2D/BuildScene/InventoringSelected.png"), 0, false, 0, WINDOW_HEIGHT - 90, 115, 90) {
		Singleton<Camera>::getInstance()->target.y = 3.;

		objects.addModel("grassland", grassland);

		grassland.scale(50);
		decorModels.push_back(std::ref(grassland));

		/*objects.addModel("pipeline", pipeline);
		pipeline.scale(0.75);
		pipeline.move(0, 0, 8);
		decorModels.push_back(std::ref(pipeline));

		platform.name = "platform";
		objects.addModel("platform", platform);
		decorModels.push_back(std::ref(platform));*/

		objects.addModel("launchTower", launchTower);
		launchTower.scale(0.35);
		launchTower.move(0, 0, -9.5);
		decorModels.push_back(std::ref(launchTower));

		farObjects.addModel("atmosphere", atmosphere);
		atmosphere.move(20. * Vector3d::UP);

		blueprints.push_back(new BlueprintPiece(PieceManager::getPieceData("PodA"), anchorPoints));
		blueprints.back()->move(0, 7, 0, blueprints);
		objects.addModel(blueprints.back()->pieceName + std::to_string(blueprints.back()->getID()),blueprints.back()->getModel());

		addModelGroup("objects", objects);
		anchorPoints.visibility = false;
		addModelGroup("anchors", anchorPoints);
		addFarModelGroup("farObjects", farObjects);

		EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
		EventDispatcher::registerEventObserver(SDL_KEYUP, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONUP, this);

		liftoffMission.catchesEvents = false;

		mainPanel.addPanel("MissionSelection", &missionSelectionPanel);

		missionSelection.onPress = [this](VisualComponent2D& component) {
			mainPanel.getPanel("MissionSelection").addComponent("LiftoffMission", &liftoffMission);
			liftoffMission.catchesEvents = true;
		};
		missionSelection.onUnPress = [this](VisualComponent2D& component) {
			mainPanel.getPanel("MissionSelection").removeComponent("LiftoffMission");
			liftoffMission.catchesEvents = false;
		};

		mainPanel.addPanel("inventory", &inventory);

		inventoryEaser.elementList.push_back({&inventory.pos.y});
		inventoringEaser.elementList.push_back({&inventoring.pos.y});

		inventoring.onPress = [this](VisualComponent2D& component) {
			int target = WINDOW_HEIGHT - this->inventory.background.h;

			this->inventoryEaser.setTarget(20, {target});
			this->inventoringEaser.setTarget(20, {target - inventoring.h});
		};
		inventoring.onUnPress = [this](VisualComponent2D& component) {
			this->inventoryEaser.setTarget(20, {WINDOW_HEIGHT});
			this->inventoringEaser.setTarget(20, {WINDOW_HEIGHT - inventoring.h});
		};

		mainPanel.addComponent("inventoring", &inventoring);

		mainPanel.addPanel("shop", &shop);

		shopEaser.elementList.push_back({&shop.pos.y});
		shoppingEaser.elementList.push_back({&shopping.pos.y});

        shopping.onPress = [this](VisualComponent2D& component) {
			int target = WINDOW_HEIGHT - this->shop.background.h;

			this->shopEaser.setTarget(20, {target});
			this->shoppingEaser.setTarget(20, {target - shopping.h});
        };
        shopping.onUnPress = [this](VisualComponent2D& component) {
            this->shopEaser.setTarget(20, {WINDOW_HEIGHT});
			this->shoppingEaser.setTarget(20, {WINDOW_HEIGHT - shopping.h});
        };

		mainPanel.addComponent("shopping", &shopping);

        mainPanel.addComponent("checkbox", &missionSelection);

		liftoffMission.onClick = [this](VisualComponent2D& component) {
			if (blueprints.size() == 1) {
				std::string blueprint;
				PlayerData::blueprint = saveBlueprint(*blueprints.back(), blueprint);
                PlayerData::fuelLevel = 100;

				nextScene = "LiftOffScene";
			}
		};
	}

	~BuildScene() {
		EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_KEYUP, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);

		for (BlueprintPiece* blueprintPiece : blueprints)
			delete blueprintPiece;
	}

	/// \brief Produit récursivement une chaine de caractère représentant le plan de la fusée
	/// \param piece La pièce mère
	/// \param blueprint Une chaine de caractère sur laquelle ajouter une partie du plan
	/// \return Chaine de caractère représentant le plan de la fusée
	std::string saveBlueprint(const BlueprintPiece& piece, std::string& blueprint) {
		if (blueprint.empty()) {
			blueprint += piece.pieceName;
			blueprint += ' ';
			for (std::pair<unsigned int, BlueprintPiece*> male : piece.malePieces) {
				blueprint += std::to_string(male.first);
				blueprint += ' ';
				blueprint += saveBlueprint(*male.second, blueprint);
				blueprint += ';';
			}
			blueprint += ':';
		}else {
			std::string subBlueprint;

			subBlueprint += piece.pieceName;
			subBlueprint += ' ';
			for (std::pair<unsigned int, BlueprintPiece*> male : piece.malePieces) {
				subBlueprint += std::to_string(male.first);
				subBlueprint += ' ';
				subBlueprint += saveBlueprint(*male.second, subBlueprint);
				subBlueprint += ';';
			}
			subBlueprint += ':';
			subBlueprint += std::to_string(piece.getMotherAnchor());

			return subBlueprint;
		}

		return blueprint;
	}

	/// \brief Trouve avec quoi le click de la souris collisionnne
	/// \return Objet avec qui il fait la collision s'il y a lieu
	Intersect CollisionClick(const Vector3d pos, const Vector3d click) {
		Intersect chosen;
		CollisionClickWithBlueprints(pos, click, chosen, blueprints);
		CollisionClickWithDecor(pos, click, chosen);
		return chosen;
	}

	/// \brief Trouve le blueprint qui est le plus près et qui collisionne avec le click
	/// \param pos Position de celui qui click
	/// \param click Position du click
	/// \param chosen Celui qui est le plus près de pos et qui collide
	/// \param blueprints Blueprints à tester
	void CollisionClickWithBlueprints(const Vector3d pos, const Vector3d click, Intersect& chosen, std::list<BlueprintPiece*> blueprints) {
		Intersect compare;
		for (BlueprintPiece* blueprint : blueprints) {
			compare = blueprint->isColliding(true, click - pos, pos);
			if (blueprint->malePieces.size()) {
				std::list<BlueprintPiece*> newBlueprints;
				for (std::pair<unsigned int, BlueprintPiece*> blueprintMale :blueprint->malePieces)
					newBlueprints.push_back(blueprintMale.second);
				CollisionClickWithBlueprints(pos, click, chosen, newBlueprints);
			}
			if (compare.collided) {
				compare.pos -= pos;
				if (chosen.collided) {
					if (compare.pos.getNorm() < chosen.pos.getNorm()) {
						chosen = compare;
						selected = blueprint;
					}
				}else {
					chosen = compare;
					selected = blueprint;
				}
			}
		}
	}

	/// \brief Trouve le decor qui est le plus près et qui collisionne avec le click
	/// \param pos Position de celui qui click
	/// \param click Position du click
	/// \param chosen Celui qui est le plus près de pos et qui collide
	void CollisionClickWithDecor(const Vector3d pos, const Vector3d click, Intersect& chosen) {
		Intersect compare;
		if (chosen.collided) {
			for (std::reference_wrapper<DecorModel> model : decorModels) {
				compare = model.get().isColliding(true, click - pos, pos);
				if (compare.collided) {
					compare.pos -= pos;
					if (compare.pos.getNorm() < chosen.pos.getNorm()) {
						selected = nullptr;
						chosen.collided = false;
						break;
					}
				}
			}
		}
	}

	///\brief Réagit à l'événement décrit dans event.
	///\param event SDL_Event contenant l'événement qui a été fait
	void notify(SDL_Event* event) {
		switch (event->type) {
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_LALT)
					anchorPoints.visibility = true;
				break;
			case SDL_KEYUP:
				if (event->key.keysym.sym == SDLK_LALT)
					anchorPoints.visibility = false;
				break;
			case SDL_MOUSEMOTION:
				if (selected) {
					selected->move(Singleton<Camera>::getInstance()->shiftScreenToFrustumPlane(event->motion.xrel, event->motion.yrel, offset), blueprints);
					selected->checkYCollision(grassland.getPos().y);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event->button.button == SDL_BUTTON_LEFT) {
					Vector3d pos = Singleton<Camera>::getInstance()->pos;
					Vector3d click = Singleton<Camera>::getInstance()->screenToFrustumPlane(event->button.x, event->button.y, FRUSTUM_NEAR);
					Intersect chosen;
					if ((chosen = CollisionClick(pos, click)).collided) {
						if (selected->detachFromMother())
							blueprints.push_back(selected);
						offset = (Singleton<Camera>::getInstance()->target - Singleton<Camera>::getInstance()->pos).normalize() * chosen.pos;
					}
				}

				break;
			case SDL_MOUSEBUTTONUP:
				if (event->button.button == SDL_BUTTON_LEFT)
					if (selected) {
						if (inventory.isRemoving() && PieceManager::getPieceData(selected->pieceName).type != "Cockpit") {

							selected->addToInventory([this](const std::string& pieceName) {
								if (!this->inventory.addPiece(pieceName)) {
									PlayerData::addItem(pieceName);
									this->inventory.addPiecePanel(pieceName, PlayerData::inventory[pieceName], this->blueprints, this->anchorPoints, this->objects);
								}
							});

							blueprints.remove(selected);
							selected->removeModel(objects, anchorPoints);
							delete selected;
						}else {
							BlueprintPiece* first = nullptr;
							BlueprintPiece* second = nullptr;
							std::pair<unsigned int, AnchorPoint>* anchorFirst = nullptr;
							std::pair<unsigned int, AnchorPoint>* anchorSecond = nullptr;
							bool tooMany = false;
							for (BlueprintPiece* blueprint : blueprints)
								findReadyToBindAnchors(first, second, anchorFirst, anchorSecond, blueprint, tooMany);
							if (!tooMany)
								if (anchorSecond) {
									if (anchorFirst->second.isMale) {
										first->bindToMotherAnchor(*anchorFirst, *anchorSecond, second);
										blueprints.remove(first);
									}else {
										second->bindToMotherAnchor(*anchorSecond, *anchorFirst, first);
										blueprints.remove(second);
									}
								}
						}

						selected = nullptr;
					}
			default:;
		}
	}

	/// \brief Trouve les anchors ayant la couleur de confirmation du AnchorPoint
	/// \param first Premier BlueprintPiece trouvé comme étant prêt à être relié
	/// \param second Deuxième BlueprintPiece trouvé comme étant prêt à être relié
	/// \param anchorFirst AnchorPoint de first prêt à être relié
	/// \param anchorSecond AnchorPoint de second prêt à être relié
	/// \param tooMany Vrai s'il y a trop d'anchors prêts à relier
	void findReadyToBindAnchors(BlueprintPiece*& first, BlueprintPiece*& second, std::pair<unsigned int, AnchorPoint>*& anchorFirst, std::pair<unsigned int, AnchorPoint>*& anchorSecond, BlueprintPiece* analyzed, bool& tooMany) {
		for (std::pair<unsigned int, AnchorPoint>& anchor : analyzed->getAnchorPoints()) {
			if (anchor.second.getModel().getMesh().getColor() == Vector3d::UP) {
				if (!anchorFirst) {
					anchorFirst = &anchor;
					first = analyzed;
					if (anchor.second.isMale)
						anchor.second.setColor(0.0, 0.0, 1.0);
					else
						anchor.second.setColor(1.0, 0.0, 0.6);
				}else {
					if (!anchorSecond) {
						anchorSecond = &anchor;
						second = analyzed;
						if (anchor.second.isMale)
							anchor.second.setColor(0.0, 0.0, 1.0);
						else
							anchor.second.setColor(1.0, 0.0, 0.6);
					}else {
						tooMany = true;
						if (anchor.second.isMale)
							anchor.second.setColor(0.0, 0.0, 1.0);
						else
							anchor.second.setColor(1.0, 0.0, 0.6);
						break;
					}
				}
			}
		}
		for (std::pair<unsigned int, BlueprintPiece*> kid: analyzed->malePieces)
			findReadyToBindAnchors(first, second, anchorFirst, anchorSecond, kid.second, tooMany);
	}
};

#endif
