/// \brief Conteneur de VisualComponent2D
///
/// Permet de contrôler plusieur VisualComponent2D et Panels en même temps.
///

#ifndef SOURCE_PANEL_H
#define SOURCE_PANEL_H

#include <map>
#include <algorithm>
#include <SDL2/SDL.h>
#include "VisualComponent2D.h"

class Panel {
protected:
	std::map<std::string, VisualComponent2D*> components;    ///< Map de VisualComponent2D.
	std::map<std::string, Panel*> panels;    ///< Map de Panel.

public:
	Position pos;    ///< Structure donnant la position en pixel du Panel relatif à son parent.
	int priority;	///< Couche sur laquelle le Panel sera dessiné. (-1 en dessous de 0)
	bool visibility;	///< Vrai si le Panel est dessiné.

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de pos.x
	/// \param y Valeur initiale de pos.y
	Panel(const int& priority, const int& x, const int& y) : pos(x, y), priority(priority), visibility(true) {}

	/// \brief Ajoute un VisualComponent2D à une map, identifié par une chaine de caractère.
	/// \param name Chaine de caractère utilisée comme clé dans la map.
	/// \param component VisualComponent2D à ajouter.
	void addComponent(const std::string& name, VisualComponent2D* component) {
		(components[name] = component)->pos.parent = &pos;
	}

	/// \brief Obtention de la VisualComponent2D associé à name.
	/// \param name Chaine de caractère associée au VisualComponent2D.
	/// \return Le VisualComponent2D recherché.
	VisualComponent2D* getComponent(const std::string& name) {
		return components[name];
	}

	/// \brief Retire un VisualComponent2D du Panel.
	/// \param name Chaine de caractère associée au VisualComponent2D.
	void removeComponent(const std::string& name) {
		if (components.find(name) != components.end())
			components.erase(name);
	}

	/// \return Nombre de composants
	unsigned long long int getComponentNum() const {
		return components.size();
	}

	/// \brief Ajoute un Panel à une map, identifié par une chaine de caractère.
	/// \param name Chaine de caractère utilisée comme clé dans la map.
	/// \param panel Panel à ajouter.
	void addPanel(const std::string& name, Panel* panel) {
		(panels[name] = panel)->pos.parent = &pos;
	}

	/// \brief Obtention de la Panel associé à name.
	/// \param name Chaine de caractère associée au Panel.
	/// \return Le Panel recherché.
	Panel& getPanel(const std::string& name) {
		return *panels[name];
	}

	/// \brief Retire un Panel du Panel.
	/// \param name Chaine de caractère associée au Panel.
	void removePanel(const std::string& name) {
		if (panels.find(name) != panels.end())
			panels.erase(name);
	}

	/// \return Nombre de panneaux
	unsigned long long int getPanelNum() const {
		return panels.size();
	}

	/// \brief Déplace le Panel de dx en x et dy en y.
	/// \param dx Déplacement à effectuer en x.
	/// \param dy Déplacement à effectuer en y.
	void move(int dx, int dy) {
		/*for (std::pair<std::string, VisualComponent2D*> component : components)
			component.second->pos.move(dx, dy);

		for (std::pair<std::string, Panel*> panel : panels) {
			panel.second->move(dx, dy);
		}*/
		pos.move(dx, dy);
	}

	/// \brief Affiche ou cache tous les VisualComponent2D.
	/// \param Vrai pour afficher les VisualComponent2D.
	void setVisibilityAll(bool visibility) {
		for (std::pair<std::string, VisualComponent2D*> component : components)
			component.second->visibility = visibility;
	}

	/// \brief Instruction à effectué à chaque tick.
	virtual void update() {
		for (std::pair<std::string, Panel*> panel : panels)
			panel.second->update();
	}

	/// \brief Dessine les VisualComponent2D et les Panel.
	void draw() {
		std::vector<std::pair<std::string, VisualComponent2D*> > sortedComponents(components.begin(), components.end());
		std::sort(sortedComponents.begin(), sortedComponents.end(), [](std::pair<std::string, VisualComponent2D*> a, std::pair<std::string, VisualComponent2D*> b) {
			return a.second->priority < b.second->priority;
		});

		for (std::pair<std::string, VisualComponent2D*>  component : sortedComponents)
			if (component.second->visibility)
				component.second->draw();

		std::vector<std::pair<std::string, Panel*> > sortedPanels(panels.begin(), panels.end());
		std::sort(sortedPanels.begin(), sortedPanels.end(), [](std::pair<std::string, Panel*> a, std::pair<std::string, Panel*> b) {
			return a.second->priority < b.second->priority;
		});

		for (std::pair<std::string, Panel*> panel : sortedPanels)
			if (panel.second->visibility)
				panel.second->draw();
	}
};

#endif //SOURCE_PANEL_H
