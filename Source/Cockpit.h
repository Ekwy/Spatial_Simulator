/// \brief Classe de la pièce cockpit.
///
/// Classe conteant toutes les propriétés du cockpit.
///

#ifndef SOURCE_COCKPIT_H
#define SOURCE_COCKPIT_H

#include <map>
#include <list>
#include "Piece.h"
#include "PieceData.h"
#include "EventDispatcher.h"
#include "ModelGroup.h"

class Cockpit : public Piece {
private:
	double transportableMass; ///< Masse que le cockpit peut contenir

	bool parachuteActive; ///< Vrai si le parachute est activé
	Model parachute; ///< Représentation graphique du parachute
	double scaling; ///< Facteur de grandissement du parachute

	ModelGroup* objects; ///< Pointeur vers le ModelGroup de ce qui est affiché comme objets dans la scene de mission active

public:
	std::map<unsigned int, std::map<std::string, std::function<void(Piece&)>*>> keyFunctions;    ///< Map de map de functions à exécuter pour chaque touche de clavier

	Cockpit(const PieceData& pieceData, std::function<void(Piece*)>& addPiece, std::function<void(Piece*)>& removePiece, ModelGroup* objects)
			: Piece(pieceData, nullptr, addPiece, removePiece),
			  parachute("Textures3D/Piece/parachute_v2_1.obj", SDLGLContext::loadTexture("Textures3D/Piece/Skins/Parachute_v2.png"), true) {
		centerOfMass = Vector3d::ZERO;
		angularMass = 0.;
		parachuteActive = false;
		scaling = 0.3;

		this->objects = objects;

		functions.reserve(6);

		functions.emplace_back("Cockpit : Front", [this](Piece& cockpit) {
			this->addForce(100000. * SDLGLContext::getFront(), this->getPosition());
		});

		functions.emplace_back("Cockpit : Back", [this](Piece& cockpit) {
			this->addForce(-100000. * SDLGLContext::getFront(), this->getPosition());
		});

		functions.emplace_back("Cockpit : Left", [this](Piece& cockpit) {
			this->addForce(-100000. * SDLGLContext::getSide(), this->getPosition());
		});

		functions.emplace_back("Cockpit : Right", [this](Piece& cockpit) {
			this->addForce(100000. * SDLGLContext::getSide(), this->getPosition());
		});

		functions.emplace_back("Cockpit : Parachute", [this](Piece& cockpit) {
			if (!this->parachuteActive) {
				this->objects->addModel("parachute", parachute);
				parachute.move(this->getPosition() - parachute.getPos() + (this->model.getTransform().getDirection()).normalize() * scaling);
				parachute.scale(scaling);
			}
			this->parachuteActive = true;
		});

		functions.emplace_back("Abort Mission", [this](Piece& cockpit) {
			PlayerData::aborted = true;
		});
	}

	/// \brief Permet la rotation de la pièce autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	virtual void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		Piece::rotateAroundLine(axis, point, angle);

		parachute.rotateAroundLine(axis, point, angle);
	}

	/// \brief Mets à jour le cockpit à chaque tick
	void update(const double& levelRatio) {
		parachute.move(getPosition() - parachute.getPos() + (model.getTransform().getDirection()).normalize() * scaling);

		if (parachuteActive) {
			if (speed.getNorm() >= 1.) {
				if ((parachute.getTransform().getDirection() / speed).getNorm() >= 0.01)
					parachute.rotateAroundLine(parachute.getTransform().getDirection() / speed, getPosition(), -0.01);

				if ((parachute.getTransform().getDirection() / model.getTransform().getDirection()).getNorm() >= 0.01) {
					Piece::rotateAroundLine(parachute.getTransform().getDirection() / speed, getPosition(), -0.005);
					Math::rotateAroundLine(-0.005, parachute.getTransform().getDirection() / speed, getPosition(), centerOfMass);
				}
			}
			if (scaling < 4.) {
				parachute.scale(1. / scaling);
				scaling += scaling * scaling / 4;
				parachute.scale(scaling);
			}
			if (speed.getNorm() >= 20.) {
				speed -= speed * 0.15 * DELTATIME;
			}
		}

		Piece::update(levelRatio);

		if (!parachuteActive) {
			std::list<std::string> functionCalled;

			for (std::pair<unsigned int, std::map<std::string, std::function<void(Piece&)>*>> funcMap : keyFunctions)
				if (EventDispatcher::getState(funcMap.first))
					for (std::pair<std::string, std::function<void(Piece&)>*> func : funcMap.second)
						if (std::find(functionCalled.begin(), functionCalled.end(), func.first) == functionCalled.end()) {
							(*func.second)(*this);
							functionCalled.push_back(func.first);
						}
		}else {
			std::list<std::string> functionCalled;

			for (std::pair<unsigned int, std::map<std::string, std::function<void(Piece&)>*>> funcMap : keyFunctions)
				if (EventDispatcher::getState(funcMap.first))
					for (std::pair<std::string, std::function<void(Piece&)>*> func : funcMap.second)
						if (std::find(functionCalled.begin(), functionCalled.end(), func.first) == functionCalled.end() && func.first == "Abort Mission") {
							(*func.second)(*this);
							functionCalled.push_back(func.first);
						}
		}
	}
};

#endif //SOURCE_COCKPIT_H
