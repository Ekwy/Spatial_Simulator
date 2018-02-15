/// \brief Scène d'atterissage
///
/// Classe représentant la scène d'atterissage
///

#ifndef SOURCE_LANDINGSCENE_H
#define SOURCE_LANDINGSCENE_H

#include "Scene.h"
#include "PlayerData.h"
#include "PieceManager.h"
#include "MissionScene.h"
#include "Cockpit.h"
#include "Label.h"
#include "DecorModel.h"
#include "Point.h"
#include "Atmosphere.h"
#include "Stage.h"
#include "Planet.h"

class LandingScene : public MissionScene {
private:
	int landingTime = 0; ///< Temps pour l'atterrissage

	const long double earthMass1 = 59'736'000; ///< Masse de la Terre...
	const long double earthMass = 1'000'000'000; ///< Multiplicateur pour la masse de la Terre...
	const long double earthMass3 = 1'000'000'000; ///< Multiplicateur pour la masse de la Terre...
	const double earthDensity = 5515; ///<  Densité de la Terre
	ModelGroup decor; ///< Decor autour de l'atterissage
	std::list<Point> points; ///< Liste de point suivant la fusée

protected:
	/// \brief Exécute les instructions post-mise à jour, à chaque tick
	void postUpdate() {
		points.push_back({cockpit->getCenterOfMass(), Vector3d::UP});
		if (points.size() >= 51)
			points.pop_front();

		if (cockpit->speed.getNorm() == 0) {
			++landingTime;
		}

		MissionScene::postUpdate();
	}

public:
	LandingScene() : MissionScene() {

		mainPanel.addComponent("labelAltitude", &vehiculeInfos);

		planets.emplace_back(1000., "PLANETSTUFF", Vector3d::UP, Vector3d::ZERO);

		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/pipeline.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/pipeline_texture.png"), false, {0, 0, 8}, 0.75));
		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/plateform.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/plateform_texture.png"), false, Vector3d::ZERO, 0.));
		//decor.addModel(planets.back().addDecorModel("Textures3D/BuildScene/LaunchTowerNicolas.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/LaunchTower_texture.png"), false, {0, 0, -9.5}, 0.35));

		planets.back().getModel().scale(5);
		planets.back().scaleRadius(5);
		planetModelGroup.addModel("planet", planets.back().getModel());
		addModelGroup("decorEarth", decor);
		addFarModelGroup("planets", planetModelGroup);

		Vector3d offset = (4000. + planets.back().getPlanetRadius()) * Vector3d::UP;
		cockpit->move(offset);
		atmosphere.move(offset);

		Vector3d distance(cockpit->getCenterOfMass() - planets.back().getPosition());
		cockpit->speed = distance.getPerpendicular() * std::sqrt(planets.back().getGravitationalForce(*cockpit).getNorm() * distance.getNorm() / cockpit->getTotalMass());

		cockpit->rotateAroundLine(Vector3d::FRONT, {cockpit->getPosition().x, cockpit->getPosition().y, cockpit->getPosition().z}, -1.57);
		cockpit->recalculateAll();
		atmosphere.setLevel(distance.getNorm());
	}

	/// \brief Vérifie si la mission est terminée
	void checkCompletion() {
		if (landingTime >= 10) {
			PlayerData::missionSuccess = true;
			nextScene = "MissionReportScene";
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

#endif //SOURCE_LANDINGSCENE_H
