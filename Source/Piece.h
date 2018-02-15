///\brief Pièce de fusée
///Classe représentant une pièce de fusée(réacteur,conteneur d'essence,cockpit...)
///

#ifndef SOURCE_PIECE_H
#define SOURCE_PIECE_H

#include <list>
#include <vector>
#include "PhysicObject.h"
#include "Model.h"
#include "Vector3d.h"
#include "PieceData.h"
#include "PieceManager.h"
#include "SDLGLContext.h"
#include "Collidable.h"

class Piece : public PhysicObject, public Collidable {
private:
	static unsigned int maxId;    ///< Id de la prochaine pièce à créer
	std::function<void(Piece*)>& addPiece;    ///< Fonction à appeler pour ajouter une pièce à MissionScene::pieces
	std::function<void(Piece*)>& removePiece; ///< Fonction à appeler pour enlever une pièce à MissionScene::pieces

	double length[3]; ///<Longueur globale du véhicule

	/// \brief Récursif de la fonction getUltimateMother
	Piece* getUltimateMother(Piece* piece) {
		if (piece->mother) {
			return getUltimateMother(piece->mother);
		}
		return piece;
	}

	/// \brief Calcule totalMass
	void calculateTotalMass(double& totalMass) {
		totalMass += getLocalMass();
		for (Piece* malePiece : malePieces)
			malePiece->calculateTotalMass(totalMass);
	}

	/// \brief Calcule centerOfMass
	void calculateCenterOfMass(Vector3d& centerOfMass, const double& totalMass) {
		centerOfMass += (getLocalMass() / totalMass) * getPosition();
		for (Piece* malePiece : malePieces)
			malePiece->calculateCenterOfMass(centerOfMass, totalMass);
	}

	/// \brief Calcule angularMass
	void calculateAngularMass() {
		calculateLengths();
		double longestLength = getLongestLength();
		angularMass = (1. / 12.) * totalMass * longestLength * longestLength;
	}

	/// \brief Retourne le point le plus bas et le plus haut sur chaque axe du monde 3D
	/// \return Array de 3(x,y,z) de std::pair<double,double> contenant le point le plus bas et le point le plus haut de l'axe
	std::array<std::pair<double, double>, 3> calculateLowAndHigh() {
		const double* vertices = getVertices();
		std::array<std::pair<double, double>, 3> coords;
		coords[0].second = coords[0].first = vertices[0]; //X
		coords[1].second = coords[1].first = vertices[1]; //Y
		coords[2].second = coords[2].first = vertices[2]; //Z
		for (unsigned int i = 1; i < getVertexCount(); i++) {
			if (coords[0].first > vertices[i * 3])
				coords[0].first = vertices[i * 3];
			else {
				if (coords[0].second < vertices[i * 3])
					coords[0].second = vertices[i * 3];
			}

			if (coords[1].first > vertices[i * 3 + 1])
				coords[1].first = vertices[i * 3 + 1];
			else {
				if (coords[1].second < vertices[i * 3 + 1])
					coords[1].second = vertices[i * 3 + 1];
			}

			if (coords[2].first > vertices[i * 3 + 2])
				coords[2].first = vertices[i * 3 + 2];
			else {
				if (coords[2].second < vertices[i * 3 + 2])
					coords[2].second = vertices[i * 3 + 2];
			}
		}

		std::array<std::pair<double, double>, 3> compare;
		for (Piece* piece: malePieces) {
			compare = piece->calculateLowAndHigh();
			if (coords[0].first > compare[0].first)
				coords[0].first = compare[0].first;
			else if (coords[0].second < compare[0].second)
				coords[0].second = compare[0].second;

			if (coords[1].first > compare[1].first)
				coords[1].first = compare[1].first;
			else if (coords[1].second < compare[1].second)
				coords[1].second = compare[1].second;

			if (coords[2].first > compare[2].first)
				coords[2].first = compare[2].first;
			else if (coords[2].second < compare[2].second)
				coords[2].second = compare[2].second;
		}

		return coords;
	}

	/// \brief Calcule les longueurs x, y, z de la fusée
	void calculateLengths() {
		std::array<std::pair<double, double>, 3> coords = calculateLowAndHigh();
		length[0] = coords[0].second - coords[0].first;
		length[1] = coords[1].second - coords[1].first;
		length[2] = coords[2].second - coords[2].first;
	}

	/// \brief Applique les dégàts sur la pièce et toutes celles reliées
	/// \param percentageApplied Pourcentage du dégàt total appliqué
	/// \param momentum Quantité de mouvement
	void applyCollisionDamage(double damage, Piece* damageSource) {
		double receivedDamage = damage * penetrability;
		if (receivedDamage > 100) {
			durability -= receivedDamage;
			damage *= SHARED_DAMAGE;

			if (mother)
				if (mother != damageSource)
					mother->applyCollisionDamage(damage, this);

			for (Piece* malePiece: malePieces)
				if (malePiece != damageSource)
					malePiece->applyCollisionDamage(damage, this);

			if (durability <= 0)
				isExploding();
		}
	}

	/// \brief Applique la réaction de la destruction (explosion) d'une pièce
	void isExploding() {
		//TODO Start explosion animation;
		detachKids();
		detachFromMother();
		removePiece(this);
		//TODO Find distance between center of explosion and each piece
		//TODO Iterate through the ones in range and apply damage relatively to distance and explosionPower
	}

protected:
	Model model;    ///< Modèle de la pièce
	double durability;    ///< Points de vie restante à la pièce
	double maxDurability; ///< Points de vie maximale de la pièce
	double penetrability;    ///< Facteur entre 0 et 1 multiplié au dommage reçu
	Piece* mother;    ///< Pointeur vers la pièce mère
	std::list<Piece*> malePieces;    ///< Liste de pointeurs aux pièces mâle
	std::vector<std::pair<const std::string, std::function<void(Piece&)>>> functions;    ///< Liste de fonctions

	/// \return La longueur la plus longue, soit en x, y ou z
	double getLongestLength() {
		if (length[0] > length[1]) {
			if (length[0] > length[2])
				return length[0];
			else
				return length[2];
		}else {
			if (length[1] > length[2])
				return length[1];
			else
				return length[2];
		}
	}

	/// \return Un tableau de composantes de sommets
	const double* getVertices() const {
		return model.getMesh().getVertices();
	}

	/// \return Le nombre de sommets
	const unsigned int& getVertexCount() const {
		return model.getMesh().getVertexCount();
	}

	/// \brief Déplace l'enfant du vecteur movement
	/// \param movement Déplacement
	virtual void moveChild(const Vector3d& movement) {
		model.move(movement.x, movement.y, movement.z);

		for (Piece* malePiece : malePieces)
			malePiece->moveChild(movement);
	}

public:
	const std::string pieceName; ///< Nom de la pièce
	const unsigned int id;    ///< Id de la pièce

	Piece(const PieceData& pieceData, Piece* mother, std::function<void(Piece*)>& addPiece, std::function<void(Piece*)>& removePiece)
			: PhysicObject(pieceData.volume, pieceData.material.name.c_str()),
			  mother(mother),
			  model(pieceData.modelPath.c_str(), SDLGLContext::loadTexture(pieceData.texturePath.c_str()), true),
			  pieceName(pieceData.name),
			  id(maxId++),
			  addPiece(addPiece),
			  removePiece(removePiece),
			  durability(pieceData.maxDurability),
			  maxDurability(pieceData.maxDurability),
			  penetrability(pieceData.material.penetrability) {}

	/// \brief Mets à jour la pièce et ses enfants à chaque tick
	/// \param levelRatio Ratio de l'atmosphère qui détermine la distance la plus petite entre la fusée et une planète
	virtual void update(const double& levelRatio) {
		PhysicObject::update(levelRatio);

		for (Piece* malePiece : malePieces) {
			malePiece->updateMother();
		}
	}

	/// \brief Mets à jour la mère racine
	virtual void updateMother() {
		for (Piece* malePiece: malePieces)
			malePiece->updateMother();
	}


	/// \return centre de masse de la fusée
	const Vector3d& getCenterOfMass() {
		return centerOfMass;
	}

	/// \return Référence au modèle
	Model& getModel() {
		return model;
	}

	/// \brief Échange le model actuel pour le model newModel
	/// \param newModel Model qui remplacera celui en place
	void swapModel(Model newModel) {
		this->model = newModel;
	}

	/// \return Durabilité actuel de la pièce
	double getDurability() {
		return durability;
	}

	/// \return Durabilité maximale de la pièce
	double getMaxDurability() {
		return maxDurability;
	}

	/// \brief Applique les dégàts sur la pièce et toutes celles reliées
	void applyCollisionDamage() {
		Piece* ultimateMother = getUltimateMother(this);
		double damage = ultimateMother->getTotalMass() * ultimateMother->speed.getNorm() * MOMENTUM_TO_DAMAGE;

		if (damage > 100) {
			durability -= damage * penetrability;
			damage *= SHARED_DAMAGE;

			if (mother)
				mother->applyCollisionDamage(damage, this);

			for (Piece* malePiece: malePieces)
				malePiece->applyCollisionDamage(damage, this);

			if (durability <= 0)
				isExploding();
		}
	}

	/// \return Retourne la longueur de la fusée en Y
	const double& getYLength() const {
		return length[1];
	}

	/// \brief Détache la pièce de sa mère
	/// \return Ancienne mère racine
	Piece* detachFromMother() {
		if (mother) {
			Piece* ultimateMother = getUltimateMother();

			addPiece(this);
			mother->malePieces.remove(this);
			mother = nullptr;

			ultimateMother->recalculateAll();
			recalculateAll();

			speed = ultimateMother->speed;
			angularSpeed = ultimateMother->angularSpeed;

			return ultimateMother;
		}
		return NULL;
	}

	/// \brief Détache la pièce de tous ses enfants
	void detachKids() {
		if (malePieces.size()) {
			Piece* ultimateMother = getUltimateMother();

			std::list<Piece*> buffer = malePieces;

			for (Piece* malePiece: buffer) {
				malePieces.remove(malePiece);
				malePiece->mother = nullptr;

				malePiece->recalculateAll();
				addPiece(malePiece);

				malePiece->speed = ultimateMother->speed;
				malePiece->angularSpeed = ultimateMother->angularSpeed;
			}

			ultimateMother->recalculateAll();
		}
	}

	///return Mère racine de la pièce
	Piece* getUltimateMother() {
		if (mother)
			return getUltimateMother(mother);
		return this;
	}

	/// \return Vecteur position
	Vector3d getPosition() const {
		return model.getPos();
	}

	/// \brief Redéfini totalMass,centerOfMass et angularMass en les recalculant avec les nouvelles données
	void recalculateAll() {
		totalMass = 0;
		calculateTotalMass(totalMass);

		centerOfMass = Vector3d::ZERO;
		calculateCenterOfMass(centerOfMass, totalMass);

		calculateAngularMass();
	}

	/// \return Retourne la masse totale de la fusée
	const double& getTotalMass() {
		return totalMass;
	}

	/// \brief Déplace la pièce et ses enfants par le vecteur movement
	/// \param movement Déplacement
	virtual void move(const Vector3d& movement) {
		centerOfMass += movement;

		model.move(movement.x, movement.y, movement.z);

		for (Piece* malePiece : malePieces)
			malePiece->moveChild(movement);
	}

	/// \brief Permet la rotation de la pièce autour d'une ligne passant par un point
	/// \param axis Vecteur de direction parallèle à la ligne
	/// \param point Point sur la ligne
	/// \param angle Angle de rotation en radian
	virtual void rotateAroundLine(const Vector3d& axis, const Vector3d& point, const double& angle) {
		model.rotateAroundLine(axis, point, angle);

		for (Piece* malePiece : malePieces)
			malePiece->rotateAroundLine(axis, point, angle);
	}

	/// \brief Ajoute et oriente une pièce mâle en alignat les AnchorPoint
	/// \param malePiece Pièce mâle à ajouter
	/// \param fAnchor Numéro de l'AnchorPoint par auquel la pièce est attaché
	virtual void addMale(Piece* malePiece, const unsigned int& fAnchor, const unsigned int& mAnchor) {
		malePieces.push_back(malePiece);

		Vector3d anchorPos(PieceManager::getPieceData(pieceName).getAnchorPosition(fAnchor));
		Vector3d anchorNormal(PieceManager::getPieceData(pieceName).getAnchorNormal(fAnchor));

		model.getTransform().multiply(anchorPos);
		model.getTransform().rotate(anchorNormal);

		malePiece->bindToAnchor(mAnchor,
								anchorPos,
								anchorNormal);
	}

	/// \brief Bind les deux anchors ensemble
	/// \param mAnchor L'AnchorPoint mâle connecté à la pièce mère
	/// \param fAnchorPos Position de l'AnchorPoint de la pièce mère
	/// \param fAnchorNormal Normale de l'AnchorPoint de la pièce mère
	void bindToAnchor(const unsigned int& mAnchor, const Vector3d& fAnchorPos, const Vector3d& fAnchorNormal) {
		Vector3d mAnchorPos(PieceManager::getPieceData(pieceName).getAnchorPosition(mAnchor));
		Vector3d mAnchorNormal(PieceManager::getPieceData(pieceName).getAnchorNormal(mAnchor));

		model.getTransform().multiply(mAnchorPos);
		model.getTransform().rotate(mAnchorNormal);

		Vector3d offset(fAnchorPos - mAnchorPos);

		mAnchorPos += offset;
		move(offset);

		double angleBetween(Vector3d::angleBetween(mAnchorNormal, fAnchorNormal));

		if (angleBetween != PI)
			rotateAroundLine(angleBetween ? mAnchorNormal / fAnchorNormal : mAnchorNormal.getPerpendicular(), fAnchorPos, angleBetween - PI);
	}

	/// \brief Modifie le vecteur movement pour refléter le mouvement maximal de la fusée
	/// \param obj Collidable auquel tester la collision
	/// \param movement Mouvement maximal désiré avec la pièce collisionnant avec le collidable
	virtual std::pair<double, Piece*> findMaxMovement(const Collidable& obj, Vector3d movement) {
		std::pair<double, Piece*> maxDistance;
		maxDistance.first = 1.;

		bool done = false;
		for (unsigned int i = 0; i < getVertexCount() * 3 && !done; i += 3) {
			Intersect intersect(obj.isColliding(false, movement, {getVertices()[i], getVertices()[i + 1], getVertices()[i + 2]}));
			if (intersect.collided) {
				if (Math::areNearlyEqual(intersect.r, 0., SMALL_DOUBLE)) {
					maxDistance.first = 0.;
					done = true;
				}else if (intersect.r < maxDistance.first) {
					maxDistance.first = intersect.r;
					maxDistance.second = this;
				}
			}
		}


		for (Piece* malePiece : malePieces)
			malePiece->findMaxMovement(obj, movement, maxDistance);

		return maxDistance;
	}

	/// \brief Permet de savoir si une ligne colinéaire à un vecteur intersecte un objet 3d
	/// \param infiniteLength Vrai si le vecteur doit être interprété comme une ligne
	/// \param direction Vecteur de direction
	/// \param position Position du vecteur
	/// \return Point d'intersection le plus proche entre le vecteur et le mesh
	Intersect isColliding(const bool& infiniteLength, const Vector3d& direction, const Vector3d& position) const {
		return model.getMesh().getAABB().intersects(infiniteLength, direction, position)
			   ? isCollidingVector(infiniteLength, model.getMesh().getVertexCount(), model.getMesh().getVertices(), model.getMesh().getNormals(), direction, position)
			   : Intersect();
	}

	/// \brief Charge les fonctions de Piece et des malePiece dans FunctionList, KeyList et Cockpit
	/// \param functionLoader Fonction de FunctionList chargeant les fonctions dans FunctionList, KeyList et Cockpit
	void loadFunctions(std::function<void(const std::string&, Piece&, std::function<void(Piece&)>*)> functionLoader) {
		for (auto& function : functions)
			functionLoader(function.first, *this, &function.second);

		for (Piece* malePiece : malePieces)
			malePiece->loadFunctions(functionLoader);
	}

	/// \brief Décharge les fonctions de Piece et des malePiece de FunctionList, KeyList et Cockpit
	/// \param functionUnloader Fonction retirant les fonctions de FunctionList, KeyList et Cockpit
	void unloadFunctions(std::function<void(const std::string&)> functionUnloader) {
		for (const auto& function : functions)
			functionUnloader(function.first);

		for (Piece* malePiece : malePieces)
			malePiece->unloadFunctions(functionUnloader);
	}

	/// \brief Modifie le vecteur movement pour refléter le mouvement maximal de la fusée
	/// \param obj Collidable auquel tester la collision
	/// \param movement Mouvement maximal désiré avec la pièce collisionnant avec le collidable
	/// \param maxDistance Pair contenant le pourcentage maximale de déplacement par rapport au movement et la pièce collisionnant
	void findMaxMovement(const Collidable& obj, Vector3d movement, std::pair<double, Piece*>& maxDistance) {
		bool done = false;
		for (unsigned int i = 0; i < getVertexCount() * 3 && !done; i += 3) {
			Intersect intersect(obj.isColliding(false, movement, {getVertices()[i], getVertices()[i + 1], getVertices()[i + 2]}));
			if (intersect.collided) {
				if (Math::areNearlyEqual(intersect.r, 0., SMALL_DOUBLE)) {
					maxDistance.first = 0.;
					done = true;
				}else if (intersect.r < maxDistance.first) {
					maxDistance.first = intersect.r;
					maxDistance.second = this;
				}
			}
		}


		for (Piece* malePiece : malePieces)
			malePiece->findMaxMovement(obj, movement, maxDistance);
	}

	/// \brief Remets maxId à 0
	static void resetIds() {
		maxId = 0;
	}
};

unsigned int Piece::maxId = 0;

#endif
