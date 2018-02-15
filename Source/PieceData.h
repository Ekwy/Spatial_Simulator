#ifndef SOURCE_PIECEDATA_H
#define SOURCE_PIECEDATA_H

#include <vector>
#include <map>
#include "Vector3d.h"
#include "Material.h"

class PieceData {
private:
	std::vector<std::tuple<bool, unsigned int, const Vector3d, const Vector3d>> anchors;	///< Map de std::pair de Vector3d position et normal des AnchorPoint

public:
	const std::string name;	///< Nom de la pièce
	const std::string type;	///< Type de la pièce
	const std::string modelPath;	///< Chemin vers le modèle
	const std::string texturePath;	///< Chemin vers la texture
	const bool hasHitbox;	///< Vrai si une Hitbox être attaché à la Piece
	const double volume;	///< Volume de la pièce
	const Material& material;	///< Matériel de la pièce
	const double maxDurability;	///< Points de vie maximum de la pièce
	const double price; ///< Prix de la piece ($)
	std::map<std::string, double> data;	//Temporary, until we find a better way to manage extra data

	PieceData(const std::string& name, const std::string& type, const std::string& modelPath,  const std::string& texturePath, const bool& hasHitbox, const double& volume, const Material& material, const double& maxDurability, const double& price)
			: name(name),
			  type(type),
			  modelPath(modelPath),
			  texturePath(texturePath),
			  hasHitbox(hasHitbox),
			  volume(volume),
			  material(material),
			  maxDurability(maxDurability),
			  price(price) {}

	/// \param i Numéro du Anchor
	/// \return Position de l'anchor i
    const Vector3d& getAnchorPosition(unsigned int i) const {
        return std::get<2>(anchors[i]);
    }

	/// \param i Numéro du Anchor
	/// \return Normal de l'anchor i
    const Vector3d& getAnchorNormal(unsigned int i) const {
        return std::get<3>(anchors[i]);
    }

	/// \return anchors
	const std::vector<std::tuple<bool, unsigned int, const Vector3d, const Vector3d>>& getAnchors() const {
		return anchors;
	}

	/// \brief Ajoute un Anchor
	/// \param isMale Vrai si mâle
	/// \param socket Type de Socket
	/// \param pos Position du Anchor
	/// \param normal Normal du Anchor
	void addAnchor(bool isMale, unsigned int socket, Vector3d pos, Vector3d normal) {
		anchors.push_back(std::make_tuple(isMale, socket, pos, normal));
	}
};

#endif //SOURCE_PIECEDATA_H