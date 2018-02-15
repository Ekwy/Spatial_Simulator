/// \brief Groupe de référence de Model

#ifndef SOURCE_MODELGROUP_H
#define SOURCE_MODELGROUP_H

#include <map>
#include <functional>
#include <iostream>
#include "Model.h"
#include "AnchorPoint.h"

struct ModelGroup {
	std::map<std::string, std::reference_wrapper<const Model>> models;	///< Map de références constantes de Model
	bool visibility;	///< Vrai si les Model doivent être dessin

	ModelGroup() : visibility(true) {}

	/// \brief Ajoute une référence de Model à la map
	/// \param name Nom de la clé d'accès pour ce Model
	/// \param model Model à ajouter
	void addModel(const std::string& name, const Model& model) {
		if(models.find(name) == models.end())
			models.emplace(name, std::cref(model));
	}

	/// \brief Retire une référence de Model de la map
	/// \param name Nom de la clé d'accès pour ce Model
	/// \param model Model à retirer
	void removeModel(const std::string& name) {
		models.erase(name);
	}

	/// \brief Dessine tous les Model du ModelGroup
	void draw() const {
		if (visibility)
			for (std::pair<std::string, std::reference_wrapper<const Model>> model : models)
				model.second.get().draw();
	}
};

#endif //SOURCE_MODELGROUP_H
