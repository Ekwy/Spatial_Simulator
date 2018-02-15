#ifndef SOURCE_PIECEMANAGER_H
#define SOURCE_PIECEMANAGER_H

#include <map>
#include <fstream>
#include "PieceData.h"

class PieceManager {
private:
	static std::map<std::string, PieceData> piecesData; ///< Map de PieceData
	static unsigned int maxID; ///< Prochain ID à utiliser pour identifier la pièce

public:
	/// \brief charger les matériaux
	/// \param path Chemin du fichier texte contenant les données des matériaux
	static void loadPiecesData(const char* pathPiece) {
		std::ifstream fs(pathPiece);

		std::string strType, name, modelPath, texturePath, materialName;
		double data, maxDurability, price;
		bool hasHitbox;

		while (!fs.eof()) {
			fs >> strType;

			if (strType == "R") {
				fs >> name;
				fs >> modelPath;
				fs >> texturePath;
				fs >> std::boolalpha >> hasHitbox;
				fs >> data;
				fs >> materialName;
				fs >> maxDurability;
				fs >> price;

				piecesData.insert(std::make_pair(name, PieceData(name, "Reactor", modelPath, texturePath, hasHitbox, data, MaterialManager::getMaterial(materialName), maxDurability, price)));	//Un peu comme piecesData[name] = PieceData(name, data, material)

				fs >> strType;

				while (strType[0] == '+') {
					fs >> data;

					piecesData.at(name).data[strType.substr(1)] = data;

					fs >> (strType = "");
				}

				while (strType == "A") {
					bool isMale;
					unsigned int socket;
					Vector3d anchorPosition;
					Vector3d anchorNormal;

					fs >> isMale;
					fs >> socket;

					fs >> anchorPosition.x;
					fs >> anchorPosition.y;
					fs >> anchorPosition.z;

					fs >> anchorNormal.x;
					fs >> anchorNormal.y;
					fs >> anchorNormal.z;

					piecesData.at(name).addAnchor(isMale, socket, anchorPosition, anchorNormal);

					fs >> (strType = "");
				}
			}

			if (strType == "C") {
				fs >> name;
				fs >> modelPath;
				fs >> texturePath;
				fs >> std::boolalpha >> hasHitbox;
				fs >> data;
				fs >> materialName;
				fs >> maxDurability;
				fs >> price;

				piecesData.insert(std::make_pair(name, PieceData(name, "Cockpit", modelPath, texturePath, hasHitbox, data, MaterialManager::getMaterial(materialName), maxDurability, price)));	//Un peu comme piecesData[name] = PieceData(name, data, material)

				fs >> strType;

				while (strType[0] == '+') {
					fs >> data;

					piecesData.at(name).data[strType.substr(1)] = data;

					fs >> (strType = "");
				}

				while (strType == "A") {
					bool isMale;
					unsigned int socket;
					Vector3d anchorPosition;
					Vector3d anchorNormal;

					fs >> isMale;
					fs >> socket;

					fs >> anchorPosition.x;
					fs >> anchorPosition.y;
					fs >> anchorPosition.z;

					fs >> anchorNormal.x;
					fs >> anchorNormal.y;
					fs >> anchorNormal.z;

					piecesData.at(name).addAnchor(isMale, socket, anchorPosition, anchorNormal);

					fs >> (strType = "");
				}
			}

			if (strType == "F") {
				fs >> name;
				fs >> modelPath;
				fs >> texturePath;
				fs >> std::boolalpha >> hasHitbox;
				fs >> data;
				fs >> materialName;
				fs >> maxDurability;
				fs >> price;

				piecesData.insert(std::make_pair(name, PieceData(name, "FuelContainer", modelPath, texturePath, hasHitbox, data, MaterialManager::getMaterial(materialName), maxDurability, price)));	//Un peu comme piecesData[name] = PieceData(name, data, material)
				fs >> strType;


				while (strType[0] == '+') {
					fs >> data;

					piecesData.at(name).data[strType.substr(1)] = data;

					fs >> (strType = "");
				}

				while (strType == "A") {
					bool isMale;
					unsigned int socket;
					Vector3d anchorPosition;
					Vector3d anchorNormal;

					fs >> isMale;
					fs >> socket;

					fs >> anchorPosition.x;
					fs >> anchorPosition.y;
					fs >> anchorPosition.z;

					fs >> anchorNormal.x;
					fs >> anchorNormal.y;
					fs >> anchorNormal.z;

					piecesData.at(name).addAnchor(isMale, socket, anchorPosition, anchorNormal);

					fs >> (strType = "");
				}
			}

			if (strType == "D") {
				fs >> name;
				fs >> modelPath;
				fs >> texturePath;
				fs >> std::boolalpha >> hasHitbox;
				fs >> data;
				fs >> materialName;
				fs >> maxDurability;
				fs >> price;

				piecesData.insert(std::make_pair(name, PieceData(name, "Decoupler", modelPath, texturePath, hasHitbox, data, MaterialManager::getMaterial(materialName), maxDurability, price)));	//Un peu comme piecesData[name] = PieceData(name, data, material)

				fs >> strType;

				while (strType[0] == '+') {
					fs >> data;

					piecesData.at(name).data[strType.substr(1)] = data;

					fs >> (strType = "");
				}

				while (strType == "A") {
					bool isMale;
					unsigned int socket;
					Vector3d anchorPosition;
					Vector3d anchorNormal;

					fs >> isMale;
					fs >> socket;

					fs >> anchorPosition.x;
					fs >> anchorPosition.y;
					fs >> anchorPosition.z;

					fs >> anchorNormal.x;
					fs >> anchorNormal.y;
					fs >> anchorNormal.z;

					piecesData.at(name).addAnchor(isMale, socket, anchorPosition, anchorNormal);

					fs >> (strType = "");
				}
			}

			if (strType == "T") {
				fs >> name;
				fs >> modelPath;
				fs >> texturePath;
				fs >> std::boolalpha >> hasHitbox;
				fs >> data;
				fs >> materialName;
				fs >> maxDurability;
				fs >> price;

				piecesData.insert(std::make_pair(name, PieceData(name, "Thruster", modelPath, texturePath, hasHitbox, data, MaterialManager::getMaterial(materialName), maxDurability, price)));	//Un peu comme piecesData[name] = PieceData(name, data, material)

				fs >> strType;

				while (strType[0] == '+') {
					fs >> data;

					piecesData.at(name).data[strType.substr(1)] = data;

					fs >> (strType = "");
				}

				while (strType == "A") {
					bool isMale;
					unsigned int socket;
					Vector3d anchorPosition;
					Vector3d anchorNormal;

					fs >> isMale;
					fs >> socket;

					fs >> anchorPosition.x;
					fs >> anchorPosition.y;
					fs >> anchorPosition.z;

					fs >> anchorNormal.x;
					fs >> anchorNormal.y;
					fs >> anchorNormal.z;

					piecesData.at(name).addAnchor(isMale, socket, anchorPosition, anchorNormal);

					fs >> (strType = "");
				}
			}
		}

		fs.close();
	}

	/// \brief Obtention d'une PieceData
	/// \param name Clé d'accès de la PieceData
	/// \return Une référence constante à la PieceData
	static const PieceData& getPieceData(const std::string& name) {
		try {
			return piecesData.at(name);
		}catch (const std::out_of_range& e) {
			std::cerr << "PieceManager::getPieceData():\n We catched an out_of_range exception with the message \"" << e.what() << "\", probably because the piece data \"" << name << "\" doesn't exist." << std::endl;
			std::exit(1);
		}
	}

	/// \brief Obtention de la map de PieceData
	/// \return Une référence constante à la map de PieceData
	static const std::map<std::string, PieceData>& getPiecesData() {
		return piecesData;
	};

	/// \param name Nom de la pièce
	/// \param numAnchor Numéro du Anchor
	/// \return Position du anchor #numAnchor dans la pièce du nom de name
	static const Vector3d& getAnchorsPositions(std::string name, unsigned int numAnchor) {
		return piecesData.at(name).getAnchorPosition(numAnchor);
	}

	/// \brief Donne l'ID pour la pièce et itère maxID
	/// \return maxID avant son incrémentation
	static unsigned int giveID() {
		return maxID++;
	}

};

std::map<std::string, PieceData> PieceManager::piecesData;
unsigned int PieceManager::maxID = 0;
#endif //SOURCE_PIECEMANAGER_H
