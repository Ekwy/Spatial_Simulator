#ifndef SDLENGINE_MAINMENU_H
#define SDLENGINE_MAINMENU_H

#include <map>
#include <functional>
#include <cmath>
#include "GUI.h"
#include "Observer.h"
#include "Battlefield.h"

class MainMenu : public GUI, public Updatable, public Observer<SDL_Event*> {
private:
	unsigned char chosenBattlefield;
	std::map<unsigned int, Team*> teams;
	unsigned int teamToDelete;
	int currPage;
	char moving;
	float timeSinceMoving;

public:
	MainMenu() : GUI(), Updatable() {
		teamToDelete = moving = timeSinceMoving = 0;
		currPage = 1;

	//Ajout des maps
		Image* background = new Image("txtArcadeBG", -2, {-64, -36, 1408, 792});
		background->addTexture(1, "txtDesertBG");
		background->addTexture(2, "txtPirateBG");
		background->addTexture(3, "txtFarmBG");
		visualComponents[-1]["imgMenuBackground"] = background;

		Image* foreground = new Image("txtArcadeMAP", -1, {-32, -18, 1344, 756});
		foreground->addTexture(1, "txtDesertMAP");
		foreground->addTexture(2, "txtPirateMAP");
		foreground->addTexture(3, "txtFarmMAP");
		visualComponents[-1]["imgMenuForeground"] = foreground;

	//Checkbox pour afficher le framerate
		visualComponents[1]["imgFrameRateOptPanel"] = new Image("txtFrameRateOptPanel", 0, WINDOWWIDTH - 210, WINDOWHEIGHT - 55);

		visualComponents[1]["lblFrameRateOpt"] = new Label("Show FPS?", "fntOpenSans", 1, {110, 110, 110, 255}, true);
		visualComponents[1]["lblFrameRateOpt"]->setPos(WINDOWWIDTH - 150, WINDOWHEIGHT - 35 - ((Label*)visualComponents[1]["lblFrameRateOpt"])->getTextHeight()/2);

		visualComponents[1]["swtFrameRateOpt"] = new Switch("txtCheckBox", "txtCheckBoxChecked", 1);
		visualComponents[1]["swtFrameRateOpt"]->setPos(WINDOWWIDTH - 200, WINDOWHEIGHT - 50);
		visualComponents[1]["swtFrameRateOpt"]->setOnClick([this](VisualComponent2D* v) {
			showFrameRate();
		});
		((Switch*)visualComponents[1]["swtFrameRateOpt"])->setOnUnClick([this](VisualComponent2D* v) {
			hideFrameRate();
		});

	//Panneau des boutons de selection de battlefield
		visualComponents[1]["imgSelectBattleFieldPanel"] = new Image("txtSelectBattlefieldPanel", 1, WINDOWWIDTH/2 - 375, WINDOWHEIGHT/2);
		visualComponents[1]["imgSelectBattleFieldHeader"] = new Image("txtSelectBattlefieldHeader", 0, WINDOWWIDTH/2 - 375, WINDOWHEIGHT/2 - 50);

		visualComponents[1]["lblSelectBattleField"] = new Label("Select your battlefield", "fntOpenSans", 1, {255, 255, 255, 255}, true);
		visualComponents[1]["lblSelectBattleField"]->setPos(WINDOWWIDTH/2 - ((Label*)visualComponents[1]["lblSelectBattleField"])->getTextWidth()/2, WINDOWHEIGHT/2 - 25 - ((Label*)visualComponents[1]["lblSelectBattleField"])->getTextHeight()/2);

	//Ajout des 4 boutons pour sélectionner les battlefield
		visualComponents[1]["btnBattlefield1"] = new Button("txtSelectBattlefieldButton", "txtSelectBattlefieldButtonPressed", 2, WINDOWWIDTH/2 - 313, WINDOWHEIGHT/2 + 40);
		((Button*)visualComponents[1]["btnBattlefield1"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield1"]->moveBy(0, 4);
			originalPositions[1]["lblBattlefield1"].second += 4;
		});
		((Button*)visualComponents[1]["btnBattlefield1"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield1"]->moveBy(0, -4);
			originalPositions[1]["lblBattlefield1"].second -= 4;
		});
		visualComponents[1]["lblBattlefield1"] = new Label("Arcade", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[1]["lblBattlefield1"]->setPos(WINDOWWIDTH/2 - 188 - ((Label*)visualComponents[1]["lblBattlefield1"])->getTextWidth()/2, WINDOWHEIGHT/2 + 75 - ((Label*)visualComponents[1]["lblBattlefield1"])->getTextHeight()/2);

		visualComponents[1]["btnBattlefield2"] = new Button("txtSelectBattlefieldButton", "txtSelectBattlefieldButtonPressed", 2, WINDOWWIDTH/2 + 63, WINDOWHEIGHT/2 + 40);
		((Button*)visualComponents[1]["btnBattlefield2"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield2"]->moveBy(0, 4);
			originalPositions[1]["lblBattlefield2"].second += 4;
		});
		((Button*)visualComponents[1]["btnBattlefield2"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield2"]->moveBy(0, -4);
			originalPositions[1]["lblBattlefield2"].second -= 4;
		});
		visualComponents[1]["lblBattlefield2"] = new Label("Desert", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[1]["lblBattlefield2"]->setPos(WINDOWWIDTH/2 + 188 - ((Label*)visualComponents[1]["lblBattlefield2"])->getTextWidth()/2, WINDOWHEIGHT/2 + 75 - ((Label*)visualComponents[1]["lblBattlefield2"])->getTextHeight()/2);

		visualComponents[1]["btnBattlefield3"] = new Button("txtSelectBattlefieldButton", "txtSelectBattlefieldButtonPressed", 2, WINDOWWIDTH/2 - 313, WINDOWHEIGHT/2 + 190);
		((Button*)visualComponents[1]["btnBattlefield3"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield3"]->moveBy(0, 4);
			originalPositions[1]["lblBattlefield3"].second += 4;
		});
		((Button*)visualComponents[1]["btnBattlefield3"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield3"]->moveBy(0, -4);
			originalPositions[1]["lblBattlefield3"].second -= 4;
		});
		visualComponents[1]["lblBattlefield3"] = new Label("Pirate", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[1]["lblBattlefield3"]->setPos(WINDOWWIDTH/2 - 188 - ((Label*)visualComponents[1]["lblBattlefield3"])->getTextWidth()/2, WINDOWHEIGHT/2 + 225 - ((Label*)visualComponents[1]["lblBattlefield3"])->getTextHeight()/2);

		visualComponents[1]["btnBattlefield4"] = new Button("txtSelectBattlefieldButton", "txtSelectBattlefieldButtonPressed", 2, WINDOWWIDTH/2 + 63, WINDOWHEIGHT/2 + 190);
		((Button*)visualComponents[1]["btnBattlefield4"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield4"]->moveBy(0, 4);
			originalPositions[1]["lblBattlefield4"].second += 4;
		});
		((Button*)visualComponents[1]["btnBattlefield4"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[1]["lblBattlefield4"]->moveBy(0, -4);
			originalPositions[1]["lblBattlefield4"].second -= 4;
		});
		visualComponents[1]["lblBattlefield4"] = new Label("Farm", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[1]["lblBattlefield4"]->setPos(WINDOWWIDTH/2 + 188 - ((Label*)visualComponents[1]["lblBattlefield4"])->getTextWidth()/2, WINDOWHEIGHT/2 + 225 - ((Label*)visualComponents[1]["lblBattlefield4"])->getTextHeight()/2);


		//onMouseOver pour les 4 boutons
		visualComponents[1]["btnBattlefield1"]->setOnMouseOver([this](VisualComponent2D* v) {
			visualComponents[-1]["imgMenuBackground"]->currTextureName = 0;
			visualComponents[-1]["imgMenuForeground"]->currTextureName = 0;
		});
		visualComponents[1]["btnBattlefield2"]->setOnMouseOver([this](VisualComponent2D* v) {
			visualComponents[-1]["imgMenuBackground"]->currTextureName = 1;
			visualComponents[-1]["imgMenuForeground"]->currTextureName = 1;
		});
		visualComponents[1]["btnBattlefield3"]->setOnMouseOver([this](VisualComponent2D* v) {
			visualComponents[-1]["imgMenuBackground"]->currTextureName = 2;
			visualComponents[-1]["imgMenuForeground"]->currTextureName = 2;
		});
		visualComponents[1]["btnBattlefield4"]->setOnMouseOver([this](VisualComponent2D* v) {
			visualComponents[-1]["imgMenuBackground"]->currTextureName = 3;
			visualComponents[-1]["imgMenuForeground"]->currTextureName = 3;
		});

	//onClick pour les 4 boutons
		visualComponents[1]["btnBattlefield1"]->setOnClick([this](VisualComponent2D* v) {
			moving = 1;
			applyToGroup(1, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[2])
				it.second->dstRect.x = originalPositions[2][it.first].first + WINDOWWIDTH;

			addVisCompsToRenderer(2);
			chosenBattlefield = 0;
		});
		visualComponents[1]["btnBattlefield2"]->setOnClick([this](VisualComponent2D* v) {
			moving = 1;
			applyToGroup(1, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[2])
				it.second->dstRect.x = originalPositions[2][it.first].first + WINDOWWIDTH;

			addVisCompsToRenderer(2);
			chosenBattlefield = 1;
		});
		visualComponents[1]["btnBattlefield3"]->setOnClick([this](VisualComponent2D* v) {
			moving = 1;
			applyToGroup(1, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[2])
				it.second->dstRect.x = originalPositions[2][it.first].first + WINDOWWIDTH;

			addVisCompsToRenderer(2);
			chosenBattlefield = 2;
		});
		visualComponents[1]["btnBattlefield4"]->setOnClick([this](VisualComponent2D* v) {
			moving = 1;
			applyToGroup(1, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[2])
				it.second->dstRect.x = originalPositions[2][it.first].first + WINDOWWIDTH;

			addVisCompsToRenderer(2);
			chosenBattlefield = 3;
		});

	//Bouton de map de test de pente
		visualComponents[1]["btnBattlefield5"] = new Image("txtSelectBattlefieldButton", 2, {635, 280, 30, 25});
		static_cast<Image*>(visualComponents[1]["btnBattlefield5"])->setSrcRect({0, 0, 0, 0});
		visualComponents[1]["btnBattlefield5"]->setOnClick([this](VisualComponent2D* v) {
			moving = 1;
			applyToGroup(1, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[2])
				it.second->dstRect.x = originalPositions[2][it.first].first + WINDOWWIDTH;

			addVisCompsToRenderer(2);
			chosenBattlefield = 4;
		});

	//Panneau de selection du nombre de joueurs par équipe
		visualComponents[2]["imgNumPlayerPanel"] = new Image("txtNumPlayerPanel", 1, WINDOWWIDTH/2 - 180, 200);
		visualComponents[2]["imgNumPlayerHeader"] = new Image("txtNumPlayerHeader", 0, WINDOWWIDTH/2 - 180, 150);
		visualComponents[2]["lblNumPlayerTitle"] = new Label("How many players per team?", "fntOpenSans", 1, {255, 255, 255, 255}, true);
		visualComponents[2]["lblNumPlayerTitle"]->setPos(WINDOWWIDTH/2 - ((Label*)visualComponents[2]["lblNumPlayerTitle"])->getTextWidth()/2, 175 - ((Label*)visualComponents[2]["lblNumPlayerTitle"])->getTextHeight()/2);

		visualComponents[2]["imgNumPlayerField"] = new Image("txtNumPlayerField", 2, WINDOWWIDTH/2 - 40, 220);
		visualComponents[2]["lblNumPlayer"] = new Label(std::to_string(Team::numPlayerPerTeam), "fntOpenSans", 3, {0, 0, 0, 255}, true);
		visualComponents[2]["lblNumPlayer"]->setPos(WINDOWWIDTH/2 - ((Label*)visualComponents[2]["lblNumPlayer"])->getTextWidth()/2, 240 - ((Label*)visualComponents[2]["lblNumPlayer"])->getTextHeight()/2);

	//Bouton pour décrémenter numPlayerPerTeam
		visualComponents[2]["lblNumPlayerLess"] = new Label("-", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[2]["lblNumPlayerLess"]->setPos(WINDOWWIDTH/2 - 68 - ((Label*)visualComponents[2]["lblNumPlayerLess"])->getTextWidth()/2, 240 - ((Label*)visualComponents[2]["lblNumPlayerLess"])->getTextHeight()/2);
		visualComponents[2]["btnNumPlayerLess"] = new Button("txtBlueCircle", "txtBlueCircle", 2, WINDOWWIDTH/2 - 86, 222);
		visualComponents[2]["btnNumPlayerLess"]->setOnClick([this](VisualComponent2D* v) {
			((Label*)visualComponents[2]["lblNumPlayer"])->setStr(std::to_string(Team::numPlayerPerTeam - 1 > 1 ? --Team::numPlayerPerTeam : 1));
			visualComponents[2]["lblNumPlayer"]->dstRect.x = WINDOWWIDTH/2 - ((Label*)visualComponents[2]["lblNumPlayer"])->getTextWidth()/2;
			originalPositions[2]["lblNumPlayer"] = std::pair<int, int>(visualComponents[2]["lblNumPlayer"]->getDstRect()->x, visualComponents[2]["lblNumPlayer"]->getDstRect()->y);
		});

	//Bouton pour incrémenter numPlayerPerTeam
		visualComponents[2]["lblNumPlayerMore"] = new Label("+", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[2]["lblNumPlayerMore"]->setPos(WINDOWWIDTH/2 + 68 - ((Label*)visualComponents[2]["lblNumPlayerMore"])->getTextWidth()/2, 240 - ((Label*)visualComponents[2]["lblNumPlayerLess"])->getTextHeight()/2);
		visualComponents[2]["btnNumPlayerMore"] = new Button("txtBlueCircle", "txtBlueCircle", 2, WINDOWWIDTH/2 + 50, 222);
		visualComponents[2]["btnNumPlayerMore"]->setOnClick([this](VisualComponent2D* v) {
			((Label*)visualComponents[2]["lblNumPlayer"])->setStr(std::to_string(Team::numPlayerPerTeam + 1 <= MAX_PLAYER_COUNT ? ++Team::numPlayerPerTeam : MAX_PLAYER_COUNT));
			visualComponents[2]["lblNumPlayer"]->dstRect.x = WINDOWWIDTH/2 - ((Label*)visualComponents[2]["lblNumPlayer"])->getTextWidth()/2;
			originalPositions[2]["lblNumPlayer"] = std::pair<int, int>(visualComponents[2]["lblNumPlayer"]->getDstRect()->x, visualComponents[2]["lblNumPlayer"]->getDstRect()->y);
		});

	//Bouton retour arriere du choix du nombre de team vers le menu principal
		visualComponents[2]["lblNumPlayerCancel"] = new Label("Cancel", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[2]["lblNumPlayerCancel"]->setPos(WINDOWWIDTH/2 - 90 - ((Label*)visualComponents[2]["lblNumPlayerCancel"])->getTextWidth()/2, 320 - ((Label*)visualComponents[2]["lblNumPlayerCancel"])->getTextHeight()/2);
		visualComponents[2]["btnNumPlayerCancel"] = new Button("txtNumPlayerCancel", "txtNumPlayerCancelPressed", 2, WINDOWWIDTH/2 - 90 - 50, 320 - 25);
		((Button*)visualComponents[2]["btnNumPlayerCancel"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[2]["lblNumPlayerCancel"]->moveBy(0, 4);
			originalPositions[2]["lblNumPlayerCancel"].second += 4;
		});
		((Button*)visualComponents[2]["btnNumPlayerCancel"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[2]["lblNumPlayerCancel"]->moveBy(0, -4);
			originalPositions[2]["lblNumPlayerCancel"].second -= 4;
		});
		visualComponents[2]["btnNumPlayerCancel"]->setOnClick([this](VisualComponent2D* v) {
			moving = -1;
			applyToGroup(2, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[1])
				it.second->dstRect.x = originalPositions[1][it.first].first - WINDOWWIDTH;

			addVisCompsToRenderer(1);
		});

	//Bouton qui passe à l'écran de création d'équipes, ajoute une équipe automatiquement
		visualComponents[2]["lblNumPlayerNext"] = new Label("Next", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[2]["lblNumPlayerNext"]->setPos(WINDOWWIDTH/2 + 90 - ((Label*)visualComponents[2]["lblNumPlayerNext"])->getTextWidth()/2, 320 - ((Label*)visualComponents[2]["lblNumPlayerNext"])->getTextHeight()/2);
		visualComponents[2]["btnNumPlayerNext"] = new Button("txtNumPlayerNext", "txtNumPlayerNextPressed", 2, WINDOWWIDTH/2 + 90 - 50, 320 - 25);
		((Button*)visualComponents[2]["btnNumPlayerNext"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[2]["lblNumPlayerNext"]->moveBy(0, 4);
			originalPositions[2]["lblNumPlayerNext"].second += 4;
		});
		((Button*)visualComponents[2]["btnNumPlayerNext"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[2]["lblNumPlayerNext"]->moveBy(0, -4);
			originalPositions[2]["lblNumPlayerNext"].second -= 4;
		});
		visualComponents[2]["btnNumPlayerNext"]->setOnClick([this](VisualComponent2D* v) {
			moving = 1;
			applyToGroup(2, [](VisualComponent2D* v) {v->hidden = true;});

			for (std::pair<std::string, VisualComponent2D*> it : visualComponents[3])
				it.second->dstRect.x = originalPositions[3][it.first].first + WINDOWWIDTH;

			addVisCompsToRenderer(3);
			visualComponents[3]["btnAddTeam"]->getOnClick()(visualComponents[3]["btnAddTeam"]);
			visualComponents[3]["btnAddTeam"]->getOnClick()(visualComponents[3]["btnAddTeam"]);
		});

	//Panneau de création des équipes
		visualComponents[3]["imgTeamPanel"] = new Image("txtTeamPanel", 1, WINDOWWIDTH/2 - 250, 150);
		visualComponents[3]["imgTeamHeader"] = new Image("txtTeamHeader", 0, WINDOWWIDTH/2 - 250, 100);
		visualComponents[3]["lblTeamTitle"] = new Label("Create teams", "fntOpenSans", 1, {255, 255, 255, 255}, true);
		visualComponents[3]["lblTeamTitle"]->setPos(WINDOWWIDTH/2 - ((Label*)visualComponents[3]["lblTeamTitle"])->getTextWidth()/2, 125 - ((Label*)visualComponents[3]["lblTeamTitle"])->getTextHeight()/2);

	//Bouton d'ajout d'équipe
		visualComponents[3]["lblAddTeam"] = new Label("Add Team", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[3]["lblAddTeam"]->setPos(WINDOWWIDTH/2 - 150 - ((Label*)visualComponents[3]["lblAddTeam"])->getTextWidth()/2, 200 - ((Label*)visualComponents[3]["lblAddTeam"])->getTextHeight()/2);
		visualComponents[3]["btnAddTeam"] = new Button("txtTeamAddButton", "txtTeamAddButtonPressed", 2, WINDOWWIDTH/2 - 225, 175);
		((Button*)visualComponents[3]["btnAddTeam"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[3]["lblAddTeam"]->moveBy(0, 4);
			originalPositions[3]["lblAddTeam"].second += 4;
		});
		((Button*)visualComponents[3]["btnAddTeam"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[3]["lblAddTeam"]->moveBy(0, -4);
			originalPositions[3]["lblAddTeam"].second -= 4;
		});
		visualComponents[3]["btnAddTeam"]->setOnClick([&, this](VisualComponent2D* v) {
			int currTeamNum = teams.size() + 1;

			if (currTeamNum <= 6) {
				std::string currTeamNumStr = std::to_string(currTeamNum);
				teams[currTeamNum] = new Team("Team " + currTeamNumStr, {255, 36, 0, 255});

				visualComponents[3]["imgPanelTeam" + currTeamNumStr] = new Image("txtTeamSubPanel", 2, WINDOWWIDTH/2 - 225, 175 + 55*(currTeamNum - 1));
				originalPositions[3]["imgPanelTeam" + currTeamNumStr] = std::pair<int, int>(WINDOWWIDTH/2 - 225, 175 + 55*(currTeamNum - 1));

				visualComponents[3]["imgFieldTeam" + currTeamNumStr] = new Image("txtTeamField", 3, WINDOWWIDTH/2 - 175, 180 + 55*(currTeamNum - 1));
				originalPositions[3]["imgFieldTeam" + currTeamNumStr] = std::pair<int, int>(WINDOWWIDTH/2 - 175, 180 + 55*(currTeamNum - 1));
				visualComponents[3]["txtinTeam" + currTeamNumStr] = new TextInput("Team " + currTeamNumStr, "fntOpenSans", 4, {0, 0, 0, 255}, true);
				visualComponents[3]["txtinTeam" + currTeamNumStr]->setPos(WINDOWWIDTH/2 - 165, 200 + 55*(currTeamNum - 1) - ((Label*)visualComponents[3]["txtinTeam" + currTeamNumStr])->getTextHeight()/2);
				originalPositions[3]["txtinTeam" + currTeamNumStr] = std::pair<int, int>(WINDOWWIDTH/2 - 165, 200 + 55*(currTeamNum - 1) - ((Label*)visualComponents[3]["txtinTeam" + currTeamNumStr])->getTextHeight()/2);
				((TextInput*)visualComponents[3]["txtinTeam" + currTeamNumStr])->setCustomRect(visualComponents[3]["imgFieldTeam" + currTeamNumStr]->getDstRect());

				((TextInput*)visualComponents[3]["txtinTeam" + currTeamNumStr])->setOnEdit([&, currTeamNum](VisualComponent2D* v) {
					teams[(currTeamNum)]->name = ((TextInput*)v)->getStr();
				});

				visualComponents[3]["imgDeleteIconTeam" + currTeamNumStr] = new Image("txtCross", 4, WINDOWWIDTH/2 + 161, 191 + 55*(currTeamNum - 1));
				originalPositions[3]["imgDeleteIconTeam" + currTeamNumStr] = std::pair<int, int>(WINDOWWIDTH/2 + 161, 191 + 55*(currTeamNum - 1));
				visualComponents[3]["btnDeleteTeam" + currTeamNumStr] = new Button("txtTeamDeleteButton", "txtTeamDeleteButtonPressed", 3, WINDOWWIDTH/2 + 150, 180 + 55*(currTeamNum - 1));
				originalPositions[3]["btnDeleteTeam" + currTeamNumStr] = std::pair<int, int>(WINDOWWIDTH/2 + 150, 180 + 55*(currTeamNum - 1));
				((Button*)visualComponents[3]["btnDeleteTeam" + currTeamNumStr])->setOnPress([this, currTeamNumStr](VisualComponent2D* v) {
					visualComponents[3]["imgDeleteIconTeam" + currTeamNumStr]->moveBy(0, 4);
					originalPositions[3]["imgDeleteIconTeam" + currTeamNumStr].second += 4;
				});
				((Button*)visualComponents[3]["btnDeleteTeam" + currTeamNumStr])->setOnUnPress([this, currTeamNumStr](VisualComponent2D* v) {
					visualComponents[3]["imgDeleteIconTeam" + currTeamNumStr]->moveBy(0, -4);
					originalPositions[3]["imgDeleteIconTeam" + currTeamNumStr].second -= 4;
				});
				visualComponents[3]["btnDeleteTeam" + currTeamNumStr]->setOnClick([&, currTeamNum](VisualComponent2D* v) {
					teamToDelete = currTeamNum;
				});

				visualComponents[3]["lblAddTeam"]->moveBy(0, 55);
				visualComponents[3]["btnAddTeam"]->moveBy(0, 55);
				originalPositions[3]["lblAddTeam"].second += 55;
				originalPositions[3]["btnAddTeam"].second += 55;

				SDLRenderer* renderer = (SDLRenderer*)SDLResourceManager::get(RENDERER_NAME);
				renderer->addVisualComponent("imgPanelTeam" + currTeamNumStr, visualComponents[3]["imgPanelTeam" + currTeamNumStr]);
				renderer->addVisualComponent("imgFieldTeam" + currTeamNumStr, visualComponents[3]["imgFieldTeam" + currTeamNumStr]);
				renderer->addVisualComponent("txtinTeam" + currTeamNumStr, visualComponents[3]["txtinTeam" + currTeamNumStr]);
				renderer->addVisualComponent("imgDeleteIconTeam" + currTeamNumStr, visualComponents[3]["imgDeleteIconTeam" + currTeamNumStr]);
				renderer->addVisualComponent("btnDeleteTeam" + currTeamNumStr, visualComponents[3]["btnDeleteTeam" + currTeamNumStr]);

				if (currTeamNum == 2) {
					renderer->addVisualComponent("lblStartGame", visualComponents[3]["lblStartGame"]);
					renderer->addVisualComponent("btnStartGame", visualComponents[3]["btnStartGame"]);
					visualComponents[3]["lblStartGame"]->hidden = visualComponents[3]["btnStartGame"]->hidden = false;
				}
			}
		});


	//Bouton de démarrage de partie
		visualComponents[3]["lblStartGame"] = new Label("Start Game", "fntOpenSans", 3, {255, 255, 255, 255}, true);
		visualComponents[3]["lblStartGame"]->setPos(WINDOWWIDTH/2 - ((Label*)visualComponents[3]["lblStartGame"])->getTextWidth()/2, 605 - ((Label*)visualComponents[3]["lblStartGame"])->getTextHeight()/2);
		visualComponents[3]["btnStartGame"] = new Button("txtStartGameButton", "txtStartGameButtonPressed", 2, WINDOWWIDTH/2 - 100, 580);
		((Button*)visualComponents[3]["btnStartGame"])->setOnPress([this](VisualComponent2D* v) {
			visualComponents[3]["lblStartGame"]->moveBy(0, 4);
			originalPositions[3]["lblStartGame"].second += 4;
		});
		((Button*)visualComponents[3]["btnStartGame"])->setOnUnPress([this](VisualComponent2D* v) {
			visualComponents[3]["lblStartGame"]->moveBy(0, -4);
			originalPositions[3]["lblStartGame"].second -= 4;
		});
		visualComponents[3]["btnStartGame"]->setOnClick([&](VisualComponent2D* v) {
			done = true;
		});

	//Empêche les groupes 2 et 3 de réagir aux events
		applyToGroup(2, [](VisualComponent2D* v) {v->hidden = true;});
		applyToGroup(3, [](VisualComponent2D* v) {v->hidden = true;});

	//Ajoute la map Arcade comme background
		((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME))->addVisualComponent("imgMenuBackground", visualComponents[-1]["imgMenuBackground"]);
		((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME))->addVisualComponent("imgMenuForeground", visualComponents[-1]["imgMenuForeground"]);
	//Ajoute le groupe 1 au SDLRenderer
		addVisCompsToRenderer(1);

		setOriginalPositions();
		SDLEngine::registerEventObserver(SDL_MOUSEMOTION, this);
	}

	~MainMenu() {
		SDLEngine::unregisterEventObserver(SDL_MOUSEMOTION, this);
	}

	Updatable* transition() {
		Battlefield* battlefield;
		std::vector<Team*> teamVector;

		for (std::pair<unsigned int, Team*> it : teams)
			teamVector.push_back(it.second);

		switch (chosenBattlefield) {
			case 0:
				battlefield = new Battlefield("txtArcadeBG", "txtArcadeMAP", 9.8, 0.3, 4, teamVector);
				break;

			case 1:
				battlefield = new Battlefield("txtDesertBG", "txtDesertMAP", 5.5, 4.7, 8, teamVector);
				break;

			case 2:
				battlefield = new Battlefield("txtPirateBG", "txtPirateMAP", 2, 2.8, 9, teamVector);
				break;

			case 3:
				battlefield = new Battlefield("txtFarmBG", "txtFarmMAP", 8.3, 1.6, 5, teamVector);
				break;

			case 4:
				battlefield = new Battlefield("txtDesertBG", "txtSlopeTestMap", 2, 1, 0, teamVector);
				break;
		}

		delete this;
		return battlefield;
	}

	float easeInOut(float t) {
		return (t < 0.5) ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
	}

	bool update() {
		if (teamToDelete) {
			delete teams[teamToDelete];

			//À la place de supprimer le TextInput du milieu, shift les noms de TextInput en TextInput et les équipes
			while (teamToDelete < teams.size()) {
				teams[teamToDelete] = teams[teamToDelete+1];
				((TextInput*)visualComponents[3]["txtinTeam" + std::to_string(teamToDelete)])->setStr(((TextInput*)visualComponents[3]["txtinTeam" + std::to_string(teamToDelete+1)])->getStr());
				((TextInput*)visualComponents[3]["txtinTeam" + std::to_string(teamToDelete++)])->updateTexture();
			}

			teams.erase(teams.size());

			//Delete les éléments de la dernière équipe, l'enlève de SDLRenderer et efface sa place dans teams
			SDLRenderer* renderer = ((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME));
			std::string teamNumStr = std::to_string(teams.size()+1);

			renderer->removeVisualComponent("imgPanelTeam" + teamNumStr);
			delete visualComponents[3]["imgPanelTeam" + teamNumStr];
			visualComponents[3].erase("imgPanelTeam" + teamNumStr);
			originalPositions[3].erase("imgPanelTeam" + teamNumStr);
			renderer->removeVisualComponent("imgFieldTeam" + teamNumStr);
			delete visualComponents[3]["imgFieldTeam" + teamNumStr];
			visualComponents[3].erase("imgFieldTeam" + teamNumStr);
			originalPositions[3].erase("imgFieldTeam" + teamNumStr);
			renderer->removeVisualComponent("txtinTeam" + teamNumStr);
			delete visualComponents[3]["txtinTeam" + teamNumStr];
			visualComponents[3].erase("txtinTeam" + teamNumStr);
			originalPositions[3].erase("txtinTeam" + teamNumStr);
			renderer->removeVisualComponent("imgDeleteIconTeam" + teamNumStr);
			delete visualComponents[3]["imgDeleteIconTeam" + teamNumStr];
			visualComponents[3].erase("imgDeleteIconTeam" + teamNumStr);
			originalPositions[3].erase("imgDeleteIconTeam" + teamNumStr);
			renderer->removeVisualComponent("btnDeleteTeam" + teamNumStr);
			delete visualComponents[3]["btnDeleteTeam" + teamNumStr];
			visualComponents[3].erase("btnDeleteTeam" + teamNumStr);
			originalPositions[3].erase("btnDeleteTeam" + teamNumStr);

			//Monte le bouton d'ajout d'équipe
			visualComponents[3]["lblAddTeam"]->moveBy(0, -55);
			visualComponents[3]["btnAddTeam"]->moveBy(0, -55);
			originalPositions[3]["lblAddTeam"].second -= 55;
			originalPositions[3]["btnAddTeam"].second -= 55;

			//Si le nombre d'équipe baisse à 1, cache btnStartGame
			if (teams.size() == 1) {
				renderer->removeVisualComponent("lblStartGame");
				renderer->removeVisualComponent("btnStartGame");
				visualComponents[3]["lblStartGame"]->hidden = visualComponents[3]["btnStartGame"]->hidden = true;
			}

			//Reset teamToDelete
			teamToDelete = 0;
		}

		if (moving) {
			if (timeSinceMoving < 1) {
				for (std::pair<std::string, VisualComponent2D*> it : visualComponents[currPage])
					it.second->dstRect.x = originalPositions[currPage][it.first].first + std::round(-moving*WINDOWWIDTH*easeInOut(timeSinceMoving));
				for (std::pair<std::string, VisualComponent2D*> it : visualComponents[currPage + moving])
					it.second->dstRect.x = originalPositions[currPage + moving][it.first].first + moving*WINDOWWIDTH + std::round(-moving*WINDOWWIDTH*easeInOut(timeSinceMoving));
				timeSinceMoving += 1/FPS;
			}else {
				for (std::pair<std::string, VisualComponent2D*> it : visualComponents[currPage])
					it.second->dstRect.x = originalPositions[currPage][it.first].first - moving*WINDOWWIDTH;
				for (std::pair<std::string, VisualComponent2D*> it : visualComponents[currPage + moving])
					it.second->dstRect.x = originalPositions[currPage + moving][it.first].first;

				applyToGroup(currPage + moving, [](VisualComponent2D* v) {v->hidden = false;});
				hideGroup(currPage);
				currPage += moving;
				moving = timeSinceMoving = 0;
			}
		}

		return done;
	}

	void notify(SDL_Event* event) {
		if (event->type == SDL_MOUSEMOTION) {
			visualComponents[-1]["imgMenuBackground"]->setPos(-64 + 64*(1 - event->motion.x/static_cast<double>(WINDOWWIDTH)), -36 + 36*(1 - event->motion.y/static_cast<double>(WINDOWHEIGHT)));
			visualComponents[-1]["imgMenuForeground"]->setPos(-32 + 32*(1 - event->motion.x/static_cast<double>(WINDOWWIDTH)), -18 + 18*(1 - event->motion.y/static_cast<double>(WINDOWHEIGHT)));
		}
	}
};

#endif //SDLENGINE_MAINMENU_H
