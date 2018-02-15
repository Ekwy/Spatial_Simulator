/// \brief Classe de gestion des données de matériaux.

#ifndef SOURCE_MATERIALMANAGER_H
#define SOURCE_MATERIALMANAGER_H

#include <map>
#include <fstream>
#include <iostream>
#include "Material.h"

class MaterialManager {
private:
	static std::map<std::string, const Material> materials; ///< Map de matériaux

public:
	/// \brief Charge les matériaux en mémoire
	/// \param path Chemin du fichier texte contenant les données des matériaux
	static void loadMaterials(const char* path) {
		std::ifstream fs(path);

		std::string name;
		double density, hardness;

		while (!fs.eof()) {
			fs >> name >> density >> hardness;

			materials.insert(std::make_pair(name, Material(name, density, hardness)));
		}

		fs.close();
	}

	/// \brief Obtention d'un Material
	/// \param name Clé d'accès du Material
	/// \return Une référence constante au Material
	static const Material& getMaterial(std::string name) {
		try {
			return materials.at(name);
		}catch (const std::out_of_range& e) {
			std::cerr << "MaterialManager::getMaterial():\n We catched an out_of_range exception with the message \"" << e.what() << "\", probably because the material \"" << name << "\" doesn't exist." << std::endl;
			std::exit(1);
		}
	}
};

std::map<std::string, const Material> MaterialManager::materials;

#endif
