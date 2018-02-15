/// \brief Classe de la mission liftoff.
///
/// Classe décrivant le contenu de la mission liftoff.
///

#ifndef SOURCE_LIFTOFFSCENE_H
#define SOURCE_LIFTOFFSCENE_H

#include "Scene.h"
#include "PlayerData.h"
#include "PieceManager.h"
#include "MissionScene.h"
#include "Cockpit.h"
#include "Label.h"
#include "DecorModel.h"
#include "Atmosphere.h"
#include "Stage.h"
#include "Planet.h"
#include "FunctionList.h"
#include "KeyList.h"

class LiftOffScene : public MissionScene {
private:
	ModelGroup decor;///< Décors dans la scene

public:
	LiftOffScene() : MissionScene(false) {
		mainPanel.addComponent("labelAltitude", &vehiculeInfos);

		planets.emplace_back(2500., "PLANETSTUFF", Vector3d::UP, Vector3d::ZERO);

		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/pipeline.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/pipeline_texture.png"), false, {0, 0, 8}, 0.75));
		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/plateform.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/plateform_texture.png"), false, Vector3d::ZERO, 0.));
		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/LaunchTowerNicolas.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/LaunchTower_texture.png"), false, {0, 0, -9.5}, 0.35));

		planetModelGroup.addModel("planet", planets.back().getModel());
		addModelGroup("decorEarth", decor);
		addFarModelGroup("planets", planetModelGroup);

		planets.back().getModel().scale(2);
		planets.back().scaleRadius(2);

		Vector3d offset((cockpit->getYLength() + .2 + planets.back().getPlanetRadius()) * Vector3d::UP);
		cockpit->move(offset);
		atmosphere.move(offset);
	}

	/// \brief Vérifie si la mission est terminée
	void checkCompletion() {
		// TODO REMETTRE LA HAUTEUR À 200'000 + CHANGER NEXTSCENE 2'000
		if ((cockpit->getPosition() - planets.back().getPosition()).getNorm() - planets.back().getPlanetRadius() >= 2'000) {
			PlayerData::missionSuccess = true;
			nextScene = "PutIntoOrbitScene";
		}
	}

	/// \brief Vérifie si la mission est échouée
	const bool checkFailure() {
		if (MissionScene::checkFailure()) {
			PlayerData::missionSuccess = false;
			nextScene = "MissionReportScene";
			return true;
		}
		return false;
	}
};


#endif //SOURCE_LIFTOFFSCENE_H
