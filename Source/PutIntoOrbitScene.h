#ifndef SOURCE_PUTINTOORBITESCENE_H
#define SOURCE_PUTINTOORBITESCENE_H

#include "MissionScene.h"
#include "Planet.h"

class PutIntoOrbitScene : public MissionScene {
private:
	const long double earthMass1 = 59'736'000; ///< Masse de la Terre...
	const long double earthMass = 1'000'000'000; ///< Multiplicateur pour la masse de la Terre...
	const long double earthMass3 = 1'000'000'000; ///< Multiplicateur pour la masse de la Terre...
	const double earthDensity = 5515; ///<  Densité de la Terre
	double timeIntoOrbit = 0.; ///< Quantité de ticks d'affilé en orbite
	ModelGroup decor; ///< Décor de la scène

	/// \brief Exécute les instructions post-mise à jour, à chaque tick
	void postUpdate() {
		if (cockpit->speed.getNorm() > 120 &&
			((cockpit->getPosition() - planets.back().getPosition()).getNorm() - planets.back().getPlanetRadius()) > 3'000 &&
			((cockpit->getPosition() - planets.back().getPosition()).getNorm() - planets.back().getPlanetRadius()) < 5'000) {
			++timeIntoOrbit;
		}else {
			timeIntoOrbit = 0;
		}

		MissionScene::postUpdate();
	}

public:
	PutIntoOrbitScene() : MissionScene() {

		mainPanel.addComponent("labelAltitude", &vehiculeInfos);

		planets.emplace_back(2500., "PLANETSTUFF", Vector3d::UP, Vector3d::ZERO);

		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/pipeline.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/pipeline_texture.png"), false, {0, 0, 8}, 0.75));
		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/plateform.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/plateform_texture.png"), false, Vector3d::ZERO, 0.));
		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/LaunchTowerNicolas.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/LaunchTower_texture.png"), false, {0, 0, -9.5}, 0.35));

		planets.back().getModel().scale(2);
		planets.back().scaleRadius(2);
		planetModelGroup.addModel("planet", planets.back().getModel());
		addModelGroup("decorEarth", decor);
		addFarModelGroup("planets", planetModelGroup);


		cockpit->move({0., 2000. + planets.back().getPlanetRadius(), 0.});
		atmosphere.move({0., 2000. + planets.back().getPlanetRadius(), 0.});
		cockpit->speed.x = 0.;
		cockpit->speed.y = 10.;
		cockpit->speed.z = 0.;
		//cockpit->rotateAroundLine({0,0,1}, {cockpit->getPosition().x, cockpit->getPosition().y, cockpit->getPosition().z}, -1.57);
		cockpit->recalculateAll();
	}

	///< \brief Vérifie l'accomplissement d'une mission
	void checkCompletion() {
		if (timeIntoOrbit >= 100)
			nextScene = "LandingScene";
	}

	///< \brief Vérifie l'échec d'une mission
	const bool checkFailure() {
		if ((cockpit->getPosition() - planets.back().getPosition()).getNorm() - planets.back().getPlanetRadius() >= 5'000 || MissionScene::checkFailure()) {
			PlayerData::missionSuccess = false;
			nextScene = "MissionReportScene";
			return true;
		}
		return false;
	}
};

#endif //SOURCE_PUTINTOORBITESCENE_H
