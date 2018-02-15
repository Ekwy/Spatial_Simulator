/// \brief Contient les données d'un matériau

#ifndef SOURCE_MATERIAL_H
#define SOURCE_MATERIAL_H

struct Material {
	const std::string name;	///< Nom du matériau
	const double density; ///< Densité du matériau
	const double penetrability; ///< Pénétrabilité du matériau

	Material(std::string name, double density, double penetrability) : name(name), density(density), penetrability(penetrability) {}

};

#endif
