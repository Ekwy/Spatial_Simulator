#ifndef SOURCE_MISSIONSCENE_H
#define SOURCE_MISSIONSCENE_H

#include <sstream>
#include "Scene.h"
#include "Piece.h"
#include "Cockpit.h"
#include "Reactor.h"
#include "FuelContainer.h"
#include "FunctionList.h"
#include "KeyList.h"
#include "Decoupler.h"
#include "Thruster.h"
#include "Planet.h"
#include "Point.h"
#include "Flame.h"

class MissionScene : public Scene {
private:
	ModelGroup piecesModelGroup; ///< ModelGroup contenant toutes les pièces de la fusée
	std::function<void(Piece*)> addPiece; ///< Ajoute la pièce à pieces (Toutes les mères racines)
	std::function<void(Piece*)> removePiece; ///< Enlève la pièce de pieces
	std::list<Point> points; ///< Liste de points dessinés au centre de masse
	std::list<Flame> flames; ///< Flames des réacteurs
	Rect labelBackground; ///< Background du label vehiculeInfos

	/// \param name Nom de la pièce
	/// \return Une instance de la pièce
	Piece* createPiece(const std::string& name, Piece* mother) {
		std::string type(PieceManager::getPieceData(name).type);

		if (type == "Reactor") {
			flames.emplace_back(Vector3d::ZERO, 1., .3, 4.);
			return new Reactor(PieceManager::getPieceData(name), mother, addPiece, removePiece, flames.back());
		}else if (type == "FuelContainer")
			return new FuelContainer(PieceManager::getPieceData(name), mother, addPiece, removePiece);
		else if (type == "Decoupler")
			return new Decoupler(PieceManager::getPieceData(name), mother, addPiece, removePiece);
		else if (type == "Thruster")
			return new Thruster(PieceManager::getPieceData(name), mother, addPiece, removePiece);
	}

	/// \brief Fonction récursive appelée par loadBlueprint(const std::string&, ModelGroup&), ajoute les enfants de mother
	/// \param mother Pièce auquel on ajoute les enfants
	/// \param bpStream Restant du plan de fusée
	/// \param modelGroup ModelGroup pour ajouter les modèles
	void loadBlueprint(Piece& mother, std::istringstream& bpStream) {
		std::string name;
		unsigned int fAnchor, mAnchor;

		bpStream >> mAnchor;
		bpStream >> name;

		Piece* piece = createPiece(name, &mother);
		objects.addModel(piece->pieceName + std::to_string(piece->id), piece->getModel());

		bpStream.ignore();
		if (bpStream.peek() == ':')
			bpStream.ignore();
		else
			loadBlueprint(*piece, bpStream);
		bpStream >> fAnchor;

		mother.addMale(piece, fAnchor, mAnchor);

		bpStream.ignore();
		if (bpStream.peek() != ':')
			loadBlueprint(mother, bpStream);
		else
			bpStream.ignore();
	}

	/// \brief Construit la fusée à partir du plan blueprint, ajoute les modèles à modelGroup
	/// \param blueprint Chaine de caractère contenant le plan
	/// \param modelGroup ModelGroup pour ajouter les modèles
	/// \return Le cockpite de la fusée
	Cockpit* loadBlueprint(const std::string& blueprint) {
		std::istringstream bpStream(blueprint);
		std::string name;

		bpStream >> name;

		Cockpit* cockpit = new Cockpit(PieceManager::getPieceData(name), addPiece, removePiece, &objects);
		objects.addModel(cockpit->pieceName + std::to_string(cockpit->id), cockpit->getModel());

		bpStream.ignore();
		if (bpStream.peek() != ':')
			loadBlueprint(*cockpit, bpStream);

		cockpit->recalculateAll();

		return cockpit;
	}

protected:
	Cockpit* cockpit; ///< Pointeur vers le cockpit (Contrôlleur du projet)
	ModelGroup objects, ///< Objets à affichage proche
			farObjects; ///< Objets à affichage loin
	Atmosphere atmosphere; ///< Atmosphère de la scène
	FunctionList functionList; ///< Liste des fonctions disponibles
	KeyList keyList; ///< Liste des clés qui sont prêtes à être utilisées
	std::list<Planet> planets; ///< Liste des planètes contenues dans la mission
	ModelGroup planetModelGroup; ///< ModelGroup contenant toutes les planètes
	std::list<Piece*> pieces; ///< Liste des pièces racines (sans mère)
	Label vehiculeInfos; ///< Label contenant les informations relatives au véhicule

	/// \brief Vérifie si la mission est terminée
	virtual void checkCompletion() = 0;

	/// \brief Vérifie si la mission est échouée
	virtual const bool checkFailure() {
		if (cockpit)
			return false;
		return true;
	};

	/// \brief Exécute les instructions pré-mise à jour, surtout l'ajout des forces, à chaque tick
	void preUpdate() {
		Scene::preUpdate();

		for (Planet& planet : planets)
			cockpit->addForce(planet.getGravitationalForce(*cockpit), cockpit->getCenterOfMass());

		cockpit->update(atmosphere.getLevel());

		std::pair<double, Piece*> maxMovement;
		std::pair<double, Piece*> buffer;
		maxMovement.first = 1.;

		for (Planet& planet : planets) {
			buffer = cockpit->findMaxMovement(planet, cockpit->speed * DELTATIME);
			if (maxMovement.first > buffer.first)
				maxMovement = buffer;
		}

		if (maxMovement.first < 1.) {
			if (maxMovement.first > 0.)
				maxMovement.second->applyCollisionDamage();
			if (cockpit) {
				cockpit->speed *= maxMovement.first;
				cockpit->angularSpeed.set(0., 0., 0.);
			}
		}

		if (cockpit)
			for (Piece* piece : pieces) {
				for (Planet& planet : planets)
					piece->addForce(planet.getGravitationalForce(*piece), piece->getCenterOfMass());

				piece->update(atmosphere.getLevel());

				maxMovement.first = 1.;
				maxMovement.second = nullptr;

				for (Planet& planet : planets) {
					buffer = piece->findMaxMovement(planet, piece->speed * DELTATIME);
					if (maxMovement.first > buffer.first)
						maxMovement = buffer;
				}

				if (maxMovement.first < 1.) {
					if (maxMovement.first > 0.)
						maxMovement.second->applyCollisionDamage();
					piece->speed *= maxMovement.first;
					piece->angularSpeed.set(0., 0., 0.);
				}
			}
	}

	/// \brief Exécute les instructions pendant la mise à jour, surtout le mouvement selon la vitesse, à chaque tick
	void periUpdate() {
		Scene::periUpdate();
		cockpit->move(cockpit->speed * DELTATIME);
		if (cockpit->angularSpeed != Vector3d::ZERO)
			cockpit->rotateAroundLine(cockpit->angularSpeed, cockpit->getCenterOfMass(), (cockpit->angularSpeed * DELTATIME).getNorm());

		for (Piece* piece : pieces) {
			piece->move(piece->speed * DELTATIME);
			if (piece->angularSpeed != Vector3d::ZERO)
				piece->rotateAroundLine(piece->angularSpeed, piece->getCenterOfMass(), (piece->angularSpeed * DELTATIME).getNorm());
		}
		atmosphere.move(cockpit->speed * DELTATIME);
	}

	/// \brief Exécute les instructions post-mise à jour, à chaque tick
	virtual void postUpdate() {
		double newDistance;
		points.push_back({cockpit->getCenterOfMass(), Vector3d::UP});
		/*if(pieces.size())
			points.push_back({pieces.front()->getCenterOfMass(), Vector3d::UP});*/
		if (points.size() > 70)
			points.pop_front();

		double distanceFromPlanet = 1101;
		for (Planet& planet: planets) {
			newDistance = (cockpit->getPosition() - planet.getPosition()).getNorm() - planet.getPlanetRadius();
			if (newDistance < distanceFromPlanet)
				distanceFromPlanet = newDistance;
		}

		atmosphere.setLevel(newDistance);

		Singleton<Camera>::getInstance()->pos += (cockpit->getCenterOfMass() - Singleton<Camera>::getInstance()->target);
		Singleton<Camera>::getInstance()->target = cockpit->getCenterOfMass();

		checkCompletion();
		checkFailure();

		if (PlayerData::aborted) {
			PlayerData::missionSuccess = false;
			nextScene = "MissionReportScene";
		}

		vehiculeInfos.setStr("Altitude : " + std::to_string((cockpit->getCenterOfMass() - planets.back().getPosition()).getNorm() - planets.back().getPlanetRadius()) + " m | Speed : " + std::to_string(cockpit->speed/*.toString*/.getNorm()) + " m/s | Reactor Power : " + std::to_string(PlayerData::reactorPower) + "% | Fuel Level : " + std::to_string(PlayerData::fuelLevel) + "% | Cockpit : " + std::to_string(cockpit->getDurability()) + "/" + std::to_string(cockpit->getMaxDurability()));

		Scene::postUpdate();
	}

public:
	MissionScene(const bool& rememberFunctions = true)
			: Scene(),
			  atmosphere("Textures3D/BuildScene/skybox_blue.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/spacebox.png"), 5'000., 1., 100., 1'100.),
			  keyList(0, 10, 10, rememberFunctions),
			  functionList(1, WINDOW_WIDTH - 410, 10, 400, 400, keyList),
			  vehiculeInfos(-1, 4, WINDOW_HEIGHT - 24, " ", "Fonts/OpenSans/OpenSans-ExtraBold.ttf", 18, {255, 0, 0, 255}),
			  labelBackground(-1, 0, WINDOW_HEIGHT - 24, WINDOW_WIDTH, 24, false, Vector3d(0.2, 0.2, 0.2)) {
		addPanel("functionList", &functionList);
		addPanel("keyList", &keyList);

		mainPanel.addComponent("labelBackground", &labelBackground);
		addModelGroup("objects", objects);
		addFarModelGroup("farObjects", farObjects);

		farObjects.addModel("atmosphere", atmosphere);

		cockpit = loadBlueprint(PlayerData::blueprint);
		keyList.keyFunctions = &cockpit->keyFunctions;

		cockpit->loadFunctions(functionList.functionLoader);
		//cockpit->move({200., 0., 0.});

		addPiece = [this](Piece* piece) {
			this->pieces.emplace_back(piece);

			piece->unloadFunctions([this](const std::string& name) {
				this->functionList.removeFunction(name);
				this->keyList.removeFunction(name);
			});
		};

		removePiece = [this](Piece* piece) {
			this->pieces.remove(piece);
			this->objects.removeModel(piece->pieceName + std::to_string(piece->id));
			if (piece == cockpit)
				cockpit = nullptr;
			delete piece;
		};

		EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
	}

	~MissionScene() {
		EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);

		for (Piece* piece : pieces)
			delete piece;

		Piece::resetIds();
	}

	/// \brief Mise à jour de la scène
	/// \return Nom de la prochaine pièce, conserve la même si aucun changement
	const std::string& update() {
		preUpdate();
		if (!checkFailure()) {
			periUpdate();
			postUpdate();
		}

		return nextScene;
	}

	///< \brief Dessine les éléments 3D
	void draw3d() {
		Scene::draw3d();

		for (Point point : points)
			point.draw();
		for (const Flame& flame : flames)
			flame.draw();
	}

	/// \brief Notification de l'action en cours
	/// \param event Structure contenant l'information sur l'action de l'utilisateur
	void notify(SDL_Event* event) {
		if (event->key.keysym.sym == SDLK_TAB)
			functionList.visibility = keyList.visibility = !functionList.visibility;
	}
};

#endif
