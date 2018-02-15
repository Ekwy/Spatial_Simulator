/// \brief Scène représentant ce qui s'affiche à l'écran.
///
/// Classe représentant une scène. Une scène est ce que l'ensemble des composants visuels 2D ou 3D que l'écran affiche.
///

#ifndef SOURCE_SCENE_H
#define SOURCE_SCENE_H

#include "Panel.h"
#include "ModelGroup.h"
#include "Camera.h"
#include "SDLGLContext.h"
#include "VisualComponent2D.h"
#include "Singleton.h"
#include "GameData.h"

class Scene : public Observer<SDL_Event*> {
protected:
	Panel mainPanel; ///< Panel de la scène au complet
	std::string nextScene;	///< Si !nextScene.empty(), fait transition vers la scène nextScene à la fin de la prochaine update

	/// \brief Affiche les éléments 3d à l'extérieur du frustum
	virtual void draw3dFar() {
		SDLGLContext::setProjection(true, FRUSTUM_ANGLE, FRUSTUM_NEAR, 1'000'000'000);
		Singleton<Camera>::getInstance()->setView();

		for (std::pair<std::string, ModelGroup&> modelGroup : farModelGroups)
			modelGroup.second.draw();

	}

	/// \brief Affiche les éléments 3d du frustum
	virtual void draw3d() {

		SDLGLContext::set3dProjection();
		Singleton<Camera>::getInstance()->setView();

		glLightiv(GL_LIGHT0, GL_POSITION, new int[4]{0, 5, 1, 0});

		for (std::pair<std::string, ModelGroup&> modelGroup : modelGroups)
			modelGroup.second.draw();
	}

	/// \brief Affiche les éléments 2d
	virtual void draw2d() {
		SDLGLContext::set2dProjection();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		mainPanel.draw();

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}

	/// \brief Exécute les instructions pré-mise à jour, surtout l'ajout des forces, à chaque tick
	virtual void preUpdate() {
		mainPanel.update();
	}

	/// \brief Exécute les instructions pendant la mise à jour, surtout le mouvement selon la vitesse, à chaque tick
	virtual void periUpdate() {}	//https://english.stackexchange.com/questions/71986/if-pre-is-previous-post-is-after-what-is-current

	/// \brief Exécute les instructions post-mise à jour, à chaque tick
	virtual void postUpdate() {
		Singleton<Camera>::getInstance()->update();
	}

public:
	std::map<std::string, std::reference_wrapper<ModelGroup>> modelGroups; ///< Map contenant les ModelGroup à afficher
	std::map<std::string, std::reference_wrapper<ModelGroup>> farModelGroups; ///< Map contenant les ModelGroup à afficher à l'extérieur du frustum

	Scene() : mainPanel(0, 0, 0), nextScene("") {
		Singleton<Camera>::getInstance()->setCamera(Orbit, 0.1, 0.1, {21., 9., -8.}, Vector3d::ZERO, Vector3d::UP, true);
	}

	virtual ~Scene() {}

	/// \brief Mets à jour la scène à chaque tick
	virtual const std::string& update() {
		preUpdate();
		periUpdate();
		postUpdate();

		return nextScene;
	}

	/// \brief Permet d'aller chercher un pointeur vers un Panel dans mainPanel
	/// \param name Nom du Panel à retourner
	/// \return Pointeur vers le Panel
	Panel& getPanel(const std::string& name) {
		return mainPanel.getPanel(name);
	}

	/// \brief Ajoute un pointeur de Panel au mainPanel
	/// \param name Nom du Panel qui sera ajouté
	/// \param panel Pointeur vers le Panel qui sera ajouté
	void addPanel(const std::string& name, Panel* panel) {
		mainPanel.addPanel(name, panel);
	}

	/// \brief Ajoute un pointeur de Component au mainPanel
	/// \param name Nom du Component qui sera ajouté
	/// \param component Pointeur vers le Component qui sera ajouté
	void addComponent(const std::string& name, VisualComponent2D* component) {
		mainPanel.addComponent(name, component);
	}

	/// \brief Ajoute un ModelGroup à la modelGroups
	/// \param name Clé d'accès du ModelGroup
	/// \param modelGroup Le ModelGroup à ajouter
	void addModelGroup(const std::string& name, ModelGroup& modelGroup) {
		modelGroups.insert(std::make_pair(name, std::ref(modelGroup)));
	}

	/// \brief Ajoute un ModelGroup à la farModelGroups
	/// \param name Clé d'accès du ModelGroup
	/// \param modelGroup Le ModelGroup à ajouter
	void addFarModelGroup(const std::string& name, ModelGroup& modelGroup) {
		farModelGroups.insert(std::make_pair(name, std::ref(modelGroup)));
	}

	/// \brief Permet l'affichage à l'écran
	void draw() {
		SDLGLContext::clear();

		draw3dFar();
		draw3d();
		draw2d();

		SDLGLContext::refresh();
	}

	/// \brief Notification de l'action en cours
	/// \param event Structure contenant l'information sur l'action de l'utilisateur
    virtual void notify(SDL_Event* event) {};
};

#endif //SOURCE_SCENE_H
