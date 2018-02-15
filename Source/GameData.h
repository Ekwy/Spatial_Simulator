/// \brief Données de jeu
///
/// Classe contenant toutes les données pertinentes à l'exécution du programme
///

#ifndef SOURCE_GAMEDATA_H
#define SOURCE_GAMEDATA_H

#include <cmath>
#include "Math.h"
#include "MaterialManager.h"
#include "PieceManager.h"

#define FPS 30.	///< Nombre de draw() par seconde
const double DELTATIME = 1. / FPS;	///< Nombre de seconde entre chaque draw()

#define WINDOW_WIDTH 1360	///< Largeur de la fenêtre
#define WINDOW_HEIGHT 768	///< Hauteur de la fenêtre

const double ASPECT_RATIO = (double)WINDOW_HEIGHT / (double)WINDOW_WIDTH;	///< Ratio de hauteur de la fenêtre sur sa largeur

#define FRUSTUM_ANGLE 68._deg	///< Champs de vision horizontale en radian
#define FRUSTUM_NEAR 0.1	///< Distance minimale de vision
#define FRUSTUM_FAR 10000.	///< Distance maximale de vision

const double TAN_THETA_OVER_TWO = (double)std::tan(FRUSTUM_ANGLE / 2.);	///< La tangeante de la moitié de FRUSTUM_ANGLE
const double FRUSTUM_RIGHT = TAN_THETA_OVER_TWO * FRUSTUM_NEAR;	///< Moitié de la largeur du plan rapproché du frustum
const double FRUSTUM_TOP = FRUSTUM_RIGHT * ASPECT_RATIO;	///< Moitié de la hauteur du plan rapproché du frustum

#define ANCHORPOINT_RADIUS 0.2	///< Rayon des AnchorPoint

#define MOMENTUM_TO_DAMAGE 0.001	///< Ratio de conversion de quantité de mouvement en dommage
#define SHARED_DAMAGE 0.25	///< Fraction du dommage qui est propagé aux pièces adjacentes

#define G 0.0000000000667 ///< Constante gravitationnelle

/// \brief Espace de noms rassemblant les propriétés des listes
namespace List {
	const unsigned WIDTH = 300;	///< Largeur des listes
	const unsigned NODE_HEIGHT = 32;	///< Hauteur d'un élément des listes
	const unsigned SPACE_BETWEEN = 5;	///< Écart entre les éléments des listes
	const unsigned MARGIN = 10;	///< Marge autour des listes
	const unsigned INDENT = 20;	///< Indentation des éléments des listes
}

struct GameData {
	///\brief Permet de charger les données de jeu
	///\param pathMaterial Chemin pour retrouver les matériaux
	///\param pathPiece Chemin pour retrouver les pièces
	static void loadData(const char* pathMaterial, const char* pathPiece) {
        MaterialManager::loadMaterials(pathMaterial);
        PieceManager::loadPiecesData(pathPiece);
    }

	/// \brief Calcul de la distance entre le centre et le côté droit du plan éloigné à planeDistance
	/// \param planeDistance Distance du plan éloigné
	/// \return Composante droite du frustum
	double getFrustumRight(const double& planeDistance) {
		return (double)std::tan(FRUSTUM_ANGLE / 2) * planeDistance;
	}

	/// \brief Calcul de la distance entre le centre et le côté du dessus du plan éloigné à planeDistance
	/// \param planeDistance Distance du plan éloigné
	/// \return Composante haut du frustum
	double getFrustumTop(const double& planeDistance) {
		return getFrustumRight(planeDistance) * ASPECT_RATIO;
	}
};

#endif
