///\brief Scène de rapport de mission
///Classe représentant le rapport d'une mission à la fin de celle-ci
///

#ifndef SOURCE_MISSIONREPORT_H
#define SOURCE_MISSIONREPORT_H

#include "Scene.h"
#include "PlayerData.h"

class MissionReportScene : public Scene {
private:
	Panel* missionReportPanel; ///< Panel contenant les composantes 2D de la scène
	Button* closeButton; ///< Bouton pour fermer
	Button* background; ///< Pour la texture du fond
	Label* completition; ///< Label pour savoir si la mission est réussite
	Label* gain; ///< Label du gain
	double moneyGain; ///< Gain
	Label* generalDescription; ///< Label contenant les informations générales de la mission
	Label* generalDescription2; ///< Label contenant les informations générales de la mission
	Label* generalDescription3; ///< Label contenant les informations générales de la mission
	Label* generalDescription4; ///< Label contenant les informations générales de la mission
	Label* generalDescription5; ///< Label contenant les informations générales de la mission
	Label* generalDescription6; ///< Label contenant les informations générales de la mission
	std::string generalText; ///< Texte général de la mission
	std::string generalText2; ///< Texte général de la mission
	std::string generalText3; ///< Texte général de la mission
	std::string generalText4; ///< Texte général de la mission
	std::string generalText5; ///< Texte général de la mission
	std::string generalText6; ///< Texte général de la mission

public:
	MissionReportScene() {
		missionReportPanel = new Panel(0, 0, 0);
		background = new Button(SDLGLContext::loadTexture("Textures2D/reportScene.png"), SDLGLContext::loadTexture("Textures2D/reportScene.png"), 0, 0, 0, 1360, 768);
		background->catchesEvents = false;
		closeButton = new Button(SDLGLContext::loadTexture("Textures2D/ButtonUnpressed.png"), SDLGLContext::loadTexture("Textures2D/ButtonPressed.png"), 1, 640, 700, 80, 55);
		if (PlayerData::missionSuccess) {
			completition = new Label(1, 138, 88, "Victoire!", "Fonts/OpenSans/OpenSans-Regular.ttf", 42, {0, 255, 0, 255});
			moneyGain = 1000;
			generalText = "C'est un petit pas pour l'Homme, mais un grand pas pour SIM";
		}
		else {
			completition = new Label(1, 138, 88, "Echec!", "Fonts/OpenSans/OpenSans-Regular.ttf", 18, {255, 0, 0, 255});
			moneyGain = 0.;
			generalText = "Discours aux médias,";
			generalText2 = "Triste journée pour l’exploration spatiale.";
			generalText3 = "Aujourd’hui, nous avons perdu la fusée Kerbal 1 qui était à la pointe de la ";
			generalText4 = "technologie ce qui nous fera prendre du retard pour la colonisation galactique";
			generalText5 = "et aussi les sept membres de l’équipage de la fusée.";
			generalText6 = "Ce n’est pas aujourd’hui qu’on va sauver Mark Watney…";
		}
		PlayerData::aborted = false;

		gain = new Label(1, 145, 218, std::to_string(moneyGain) + " dollars en récompense.", "Fonts/OpenSans/OpenSans-Regular.ttf", 18, {0, 0, 0, 255});
		generalDescription = new Label(1, 740, 100, generalText, "Fonts/OpenSans/OpenSans-Regular.ttf", 14, {0, 0, 0, 255});
		generalDescription2 = new Label(1, 740, 160, generalText2, "Fonts/OpenSans/OpenSans-Regular.ttf", 13, {0, 0, 0, 255});
		generalDescription3 = new Label(1, 740, 180, generalText3, "Fonts/OpenSans/OpenSans-Regular.ttf", 13, {0, 0, 0, 255});
		generalDescription4 = new Label(1, 740, 200, generalText4, "Fonts/OpenSans/OpenSans-Regular.ttf", 13, {0, 0, 0, 255});
		generalDescription5 = new Label(1, 740, 220, generalText5, "Fonts/OpenSans/OpenSans-Regular.ttf", 13, {0, 0, 0, 255});
		generalDescription6 = new Label(1, 740, 280, generalText6, "Fonts/OpenSans/OpenSans-Regular.ttf", 13, {0, 0, 0, 255});



		mainPanel.addPanel("missionReportPanel", missionReportPanel);
		missionReportPanel->addComponent("background", background);
		missionReportPanel->addComponent("closeButton", closeButton);
		missionReportPanel->addComponent("completition", completition);
		missionReportPanel->addComponent("gain", gain);
		missionReportPanel->addComponent("generalDescription", generalDescription);
		missionReportPanel->addComponent("generalDescription2", generalDescription2);
		missionReportPanel->addComponent("generalDescription3", generalDescription3);
		missionReportPanel->addComponent("generalDescription4", generalDescription4);
		missionReportPanel->addComponent("generalDescription5", generalDescription5);
		missionReportPanel->addComponent("generalDescription6", generalDescription6);


		EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
		EventDispatcher::registerEventObserver(SDL_KEYUP, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEBUTTONUP, this);


		std::function<void(VisualComponent2D&)> switchScene = [this](VisualComponent2D& visComp) {
			this->nextScene = "BuildScene";
			PlayerData::money += moneyGain;
		};

		closeButton->onClick = switchScene;
	}

	~MissionReportScene() {
		delete closeButton;
		delete completition;
		delete gain;
		delete generalDescription;
		delete generalDescription2;
		delete generalDescription3;
		delete generalDescription4;
		delete missionReportPanel;
		EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_KEYUP, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
	}

};


#endif //SOURCE_MISSIONREPORT_H
