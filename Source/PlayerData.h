/// \brief Données du joueur
///
/// Classe représentant les données du joueur (Inventaire, argent, rapport de mission?)
///

#ifndef SOURCE_PLAYERDATA_H
#define SOURCE_PLAYERDATA_H

#include <map>
#include <vector>

struct PlayerData {
	static std::string blueprint;	///< Chaine de caractère représentant le plan de la fusée
	static std::map<std::string, unsigned> inventory;	///< Map de quantité d'items dans l'inventaire
	static unsigned int money; ///< Argent disponible du joueur
	static bool missionSuccess; ///< Si la mission est un succès ou non
	static std::map<unsigned, std::vector<std::string>> commands;
	static unsigned int reactorPower; ///< Puissance actuelle du réacteur (%) (unsigned int pour ne pas avoir les décimales)
	static unsigned int fuelLevel; ///< Fuel Level actuel (%)
	static bool aborted; ///< Si la mission est abandonnée

	/// \brief Ajoute une pièce name à inventory
	static void addItem(const std::string& name) {
		if (inventory.find(name) == inventory.end())
			inventory.emplace(name, 1);
		else
			inventory[name]++;
	}

	/// \brief Retire une pièce name d'inventory
	static void removeItem(const std::string& name) {
		if (inventory.find(name) != inventory.end())
			if (!(--inventory[name]))
				inventory.erase(name);
	}

};

std::string PlayerData::blueprint = "PodA 0 CheapFuelTank 0 Reacteur2 :1;:5;:";
std::map<std::string, unsigned> PlayerData::inventory;
unsigned int PlayerData::money = 250;
bool PlayerData::missionSuccess = false;
std::map<unsigned, std::vector<std::string>> PlayerData::commands;
unsigned int PlayerData::reactorPower;
unsigned int PlayerData::fuelLevel = 100;
bool PlayerData::aborted = false;

#endif
