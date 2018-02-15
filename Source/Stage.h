///\brief Classe gérant l'affichage
///
/// Classe permetant de gérer l'affichage des scènes
///

#ifndef SOURCE_STAGE_H
#define SOURCE_STAGE_H

#include <map>
#include "Scene.h"

class Stage {
private:
	static Scene* currentScene; ///<Pointeur vers la scène active à l'écran
	static std::string nextScene;	///< Stock la chaine de caractère retournée par Scene::update()

public:
	static std::map<std::string, Scene*(*)()> sceneFactory; ///< Map de références à createInstance() pour différentes scènes

	/// \brief Met à jour la scène à chaque tick
	static void update() {
		if (!(nextScene = currentScene->update()).empty())
			setCurrentScene(nextScene);
	}

	/// \return Nouvelle instance de T
	template <typename T>
	static Scene* createInstance() {	//http://stackoverflow.com/a/582456/2692695
		return new T;
	}

	///\brief Méthode permettant de définir la scène active
	///\param sceneName Nom de la scène qui deviendra la scène active
	static void setCurrentScene(const std::string& sceneName) {
		delete currentScene;
		currentScene = (*sceneFactory[sceneName])();

		SDLGLContext::setTitle("Projet d'Intégration - " + sceneName);
	}

	///\brief Méthode permettant l'affichage de la scène active
	static void draw() {
		currentScene->draw();
	}
};

Scene* Stage::currentScene = nullptr;
std::string Stage::nextScene;
std::map<std::string, Scene*(*)()> Stage::sceneFactory;

#endif
