/// \brief Classe de pièces d'une scène de construction
///
/// Classe permettant de créer des pièces servant à construire un véhicule
///

#ifndef SOURCE_BLUEPRINTPIECE_H
#define SOURCE_BLUEPRINTPIECE_H

#include <list>
#include <vector>
#include "Collidable.h"
#include "PieceData.h"
#include "AnchorPoint.h"
#include "Observer.h"
#include "Model.h"
#include "ModelGroup.h"

class BlueprintPiece : public Collidable {
private:
	Model model; ///< model de la pièce
	std::vector<std::pair<unsigned int, AnchorPoint>> anchorPoints; ///< liste d'anchorPoints de la pièce
	const unsigned int id; ///< ID de l'identificateur de la pièce
	std::pair<unsigned int, BlueprintPiece*> mother; ///< Pièce mère à laquelle la pièce s'attache et l'AnchorPoint male avec lequel elle est attachée.

	/// \return Un tableau de composantes de sommets
	const double* getVertices() const {
		return model.getMesh().getVertices();
	}

	/// \return Le nombre de sommets
	const unsigned int& getVertexCount() const {
		return model.getMesh().getVertexCount();
	}

	/// \brief Regarde la collision en Y avec collidingCoord
	/// \param collidingCoord Position en Y de la collision
	/// \param moveDistance Distance pour ramener l'objet à la collision
	void checkYCollision(double collidingCoord, double& moveDistance) {
		const double* vertices = getVertices();
		for (unsigned int i = 0; i < getVertexCount(); i++) {
			if (collidingCoord - vertices[i * 3 + 1] > moveDistance)
				moveDistance = collidingCoord - vertices[i * 3 + 1];
		}
		for (std::pair<unsigned int, BlueprintPiece*> malePiece : malePieces) {
			malePiece.second->checkYCollision(collidingCoord, moveDistance);

		}
	}

	/// \brief Détecte les collisions entre les anchors d'un Blueprint et de celui courant
	/// \param blueprintPiece Blueprint comparé
	bool detectAnchors(BlueprintPiece* blueprintPiece) {
		for (std::pair<unsigned int, AnchorPoint>& anchorPoint: anchorPoints) {
			for (std::pair<unsigned int, AnchorPoint>& anchorPoint2: blueprintPiece->getAnchorPoints())
				if (anchorPoint.second.isColliding(anchorPoint2.second))
					return true;
		}
		for(std::pair<unsigned int,BlueprintPiece*> malePiece: blueprintPiece->malePieces)
			if (detectAnchors(malePiece.second))
				return true;
		return false;
	}

public:
	std::list<std::pair<unsigned int, BlueprintPiece*>> malePieces; ///< Liste de pièces males attachées à la pièce et l'AnchorPoint auquel ils sont attaché
	const std::string pieceName; ///< Nom de la pièce

	BlueprintPiece(const PieceData& pieceData, ModelGroup& anchorGroup) : mother(std::make_pair(0, nullptr)),
																		  id(PieceManager::giveID()),
																		  pieceName(pieceData.name),
																		  model(pieceData.modelPath.c_str(), SDLGLContext::loadTexture(pieceData.texturePath.c_str()), true) {
		anchorPoints.reserve(pieceData.getAnchors().size());

		for (unsigned int i = 0; i < anchorPoints.capacity(); i++) {
			anchorPoints.emplace_back(i, pieceData.getAnchors()[i]);
			anchorGroup.addModel(pieceName + std::to_string(id) + "anchor" + std::to_string(i),anchorPoints.back().second.getModel());
		}
	}

	~BlueprintPiece() {
		for (std::pair<unsigned int, BlueprintPiece*> blueprint : malePieces)
			delete blueprint.second;
	}

	/// \brief Ajoute la pièce et ses enfants à l'inventaire
	/// \param func Fonction ajoutant la pièce à l'inventaire
	void addToInventory(std::function<void(const std::string&)> func) const {
		func(pieceName);

		for (std::pair<unsigned int, BlueprintPiece*> blueprint : malePieces)
			blueprint.second->addToInventory(func);
	}

	/// \brief Retire model et les modèles des enfants du ModelGroup
	/// \param modelGroup ModelGroup des pièces
	/// \param anchorModelGroup ModelGroup des AnchorPoints
	void removeModel(ModelGroup& modelGroup, ModelGroup& anchorModelGroup) {
		for (unsigned int i = 0; i < anchorPoints.capacity(); i++)
			anchorModelGroup.removeModel(pieceName + std::to_string(id) + "anchor" + std::to_string(i));
		modelGroup.removeModel(pieceName + std::to_string(id));

		for (std::pair<unsigned int, BlueprintPiece*> blueprint : malePieces)
			blueprint.second->removeModel(modelGroup, anchorModelGroup);
	}

	/// \brief Accès au model de la pièce
	/// \return Référence du model
	Model& getModel() {
		return model;
	}

	/// \brief Détache la pièce de sa mère
	/// \return Vrai si il y a eu détachement
	bool detachFromMother() {//TODO Reangle piece to original angling
		if (mother.second) {
			unsigned int pos;
			for (std::pair<unsigned int, AnchorPoint>& anchor : anchorPoints) {
				if (anchor.second.isMale) {
					anchor.second.binded = false;
					pos = anchor.first;
					break;
				}
			}
			mother.second->getAnchorPoints().at(mother.first).second.binded = false;
			mother.second->malePieces.remove(std::make_pair(pos, this));
			mother.second = nullptr;
			return true;
		}
		return false;
	}

	/// \brief Accès à mother
	const unsigned int& getMotherAnchor() const {
		try {
			if (mother.second)
				return mother.first;
			throw std::out_of_range("Piece:\"" + pieceName + "\" ID:\"" + std::to_string(id) + "\" has no mother.");
		}catch (std::out_of_range e) {
			std::cerr << e.what() << std::endl;
			std::exit(1);
		}
	};

	/// \brief Applique les transformation d'une matrice 4x4 à la pièce
	/// \param Matrice de transformation
	void applyTransform(const Matrix4d& mat) {
		model.applyTransform(mat);

		for (std::pair<unsigned int, AnchorPoint>& anchorPoint: anchorPoints) {
			anchorPoint.second.getModel().applyTransform(mat);
			mat.rotate(anchorPoint.second.getNormal());
		}

		for (std::pair<unsigned int, BlueprintPiece*>& blueprintPiece: malePieces)
			blueprintPiece.second->applyTransform(mat);
	}

	/// \brief Déplace le blueprintPiece et tous ses enfants en le détachant de sa mère en considérant les collisions entre points d'ancrage
	/// \param movement Déplacement en x,y,z
	/// \param blueprintPieces Map de blueprintPieces pour regarder les collisions avec leurs points d'ancrage
	void move(Vector3d movement, std::list<BlueprintPiece*> blueprintPieces) {
		move(movement.x, movement.y, movement.z, blueprintPieces);
	}

	/// \brief Déplace le blueprintPiece et tous ses enfants en le détachant de sa mère en considérant les collisions entre points d'ancrage
	/// \param x Déplacement en x
	/// \param y Déplacement en y
	/// \param z Déplacement en z
	/// \param blueprintPieces Map de blueprintPieces pour regarder les collisions avec leurs points d'ancrage
	void move(double x, double y, double z, std::list<BlueprintPiece*> blueprintPieces) {
		moveChild(x, y, z, blueprintPieces, this);

		if (mother.second) {
			mother.second->malePieces.remove_if([this](const std::pair<unsigned int, BlueprintPiece*>& a) {
				return a.second == this;
			});
			mother.second = nullptr;
		}
	}

	/// \brief Déplace le blueprintPiece et tous ses enfants sans les détacher de sa mère en considérant les collisions entre points d'ancrage
	/// \param x Déplacement en x
	/// \param y Déplacement en y
	/// \param z Déplacement en z
	/// \param blueprintPieces Map de blueprintPieces pour regarder les collisions avec leurs points d'ancrage
	/// \param mom Blueprint dans blueprints pour la composante
	void moveChild(double x, double y, double z, std::list<BlueprintPiece*> blueprintPieces, BlueprintPiece* mom) {
		model.move(x, y, z);
		for (std::pair<unsigned int, AnchorPoint>& anchorPoint: anchorPoints)
			anchorPoint.second.move(x, y, z);

		for (std::pair<unsigned int, BlueprintPiece*> malePiece: malePieces)
			malePiece.second->moveChild(x, y, z, blueprintPieces, mom);

		bool end = false;
		for (BlueprintPiece* blueprintPiece : blueprintPieces) {
			if (blueprintPiece != mom) {
				if (!(end = detectAnchors(blueprintPiece))) {
					for (std::pair<unsigned int, BlueprintPiece*> kid: blueprintPiece->malePieces)
						if (end = detectAnchors(kid.second))
							break;
				}
			}
			if (end)
				break;
		}
	}

	/// \brief Déplace le blueprintPiece et tous ses enfants en le détachant de sa mère sans considérer les collisions entre points d'ancrage
	/// \param x Déplacement en x
	/// \param y Déplacement en y
	/// \param z Déplacement en z
	/// \param blueprintPieces Map de blueprintPieces pour regarder les collisions avec leurs points d'ancrage
	void moveToBind(double x, double y, double z) {
		moveToBindChild(x, y, z);

		if (mother.second) {
			mother.second->malePieces.remove_if([this](const std::pair<unsigned int, BlueprintPiece*>& a) {
				return a.second == this;
			});
			mother.second = nullptr;
		}
	}

	/// \brief Déplace le blueprintPiece et tous ses enfants sans le détacher de sa mère sans considérer les collisions entre points d'ancrage
	/// \param x Déplacement en x
	/// \param y Déplacement en y
	/// \param z Déplacement en z
	/// \param blueprintPieces Map de blueprintPieces pour regarder les collisions avec leurs points d'ancrage
	void moveToBindChild(double x, double y, double z) {
		model.move(x, y, z);

		for (std::pair<unsigned int, AnchorPoint>& anchorPoint: anchorPoints)
			anchorPoint.second.move(x, y, z);

		for (std::pair<unsigned int, BlueprintPiece*> malePiece: malePieces)
			malePiece.second->moveToBindChild(x, y, z);
	}

	/// \brief Déplace le blueprintPiece et tous ses enfants en le détachant de sa mère sans considérer les collisions entre points d'ancrage
	/// \param movement Déplacement en x,y,z
	/// \param blueprintPieces Map de blueprintPieces pour regarder les collisions avec leurs points d'ancrage
	void moveToBind(Vector3d movement) {
		moveToBind(movement.x, movement.y, movement.z);
	}

	/// \brief Permet la rotation du BlueprintPiece autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		model.rotateAroundLine(axis, point, angle);

		for (std::pair<unsigned int, AnchorPoint>& anchor : anchorPoints)
			anchor.second.rotateAroundLine(axis, point, angle);

		for (std::pair<unsigned int, BlueprintPiece*>& blueprintPiece: malePieces)
			blueprintPiece.second->rotateAroundLine(axis, point, angle);
	}

	/// \brief Grandissement du blueprintPiece
	/// \param scaleFactor Facteur de grandissement
	void scale(double scaleFactor) {
		model.scale(scaleFactor);
		for (std::pair<unsigned int, AnchorPoint>& anchor : anchorPoints)
			anchor.second.move(anchor.second.getPos() * scaleFactor - anchor.second.getPos());
	}

	/// \brief Accès à anchorPoints
	/// \return anchorPoints
	std::vector<std::pair<unsigned int, AnchorPoint>>& getAnchorPoints() {
		return anchorPoints;
	}

	///\brief Permet de savoir si une ligne colinéaire à un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	///\param direction Vecteur de direction
	///\param position Position du vecteur
	///\return Point d'intersection le plus proche entre le vecteur et le mesh
	Intersect isColliding(const bool& infiniteLength, const Vector3d& direction, const Vector3d& position) const {
		return model.getMesh().getAABB().intersects(infiniteLength, direction, position)
			   ? isCollidingVector(infiniteLength, model.getMesh().getVertexCount(), model.getMesh().getVertices(), model.getMesh().getNormals(), direction, position)
			   : Intersect();
	};

	/// \brief Bind le anchor enfant au anchor mère
	/// \param mAnchor L'AnchorPoint mâle connecté à la pièce mère
	/// \param fAnchor L'AnchorPoint femelle connecté à l'enfant
	/// \param motherOrientation Orientation de la pièce mère
	void bindToMotherAnchor(std::pair<unsigned int, AnchorPoint>& mAnchor, std::pair<unsigned int, AnchorPoint>& fAnchor, BlueprintPiece* mother) {
		applyTransform(Math::getInverseRotation(model.getTransform()));

		moveToBind(fAnchor.second.getPos() - mAnchor.second.getPos());

		double angleBetween(Vector3d::angleBetween(mAnchor.second.getNormal(), fAnchor.second.getNormal()));

		if (angleBetween != PI)
			rotateAroundLine(angleBetween ? mAnchor.second.getNormal() / fAnchor.second.getNormal() : mAnchor.second.getNormal().getPerpendicular(), fAnchor.second.getPos(), angleBetween - PI);

		this->mother.first = fAnchor.first;
		this->mother.second = mother;

		fAnchor.second.binded = mAnchor.second.binded = true;

		mother->malePieces.push_back(std::make_pair(mAnchor.first, this));
	}

	/// \brief Regarde la collision en Y avec collidingCoord
	/// \param collidingCoord Position en Y de la collision
	void checkYCollision(double collidingCoord) {
		const double* vertices = getVertices();
		double moveDistance = 0;

		for (unsigned int i = 0; i < getVertexCount(); i++)
			if (collidingCoord - vertices[i * 3 + 1] > moveDistance)
				moveDistance = collidingCoord - vertices[i * 3 + 1];

		for (std::pair<unsigned int, BlueprintPiece*> malePiece : malePieces)
			malePiece.second->checkYCollision(collidingCoord, moveDistance);

		moveToBind(0, moveDistance, 0);
	}

	/// \return id
	const unsigned int& getID(){
		return id;
	}
};

#endif
