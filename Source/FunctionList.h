/// \brief Panel contenant la liste de fonctions des pièces de la fusée

#ifndef SOURCE_FUNCTIONLIST_H
#define SOURCE_FUNCTIONLIST_H

#include <map>
#include <list>
#include <functional>
#include "Panel.h"
#include "KeyList.h"
#include "Piece.h"
#include "Cockpit.h"
#include "Button.h"

class FunctionList : public Panel, public Observer<SDL_Event*> {
private:
	/// \brief Panel représentant une fonction dans la liste
	struct FunctionPanel : public Panel {
		std::string name;	///< Nom de la fonction
		Button background;	///< Fond et bouton du FunctionPanel
		Label label;	///< Label du FunctionPanel

		/// \param w Largeur du FunctionPanel
		/// \param numFunctionPanel Nombre de FunctionPanel avant celui-ci
		FunctionPanel(const std::string& name, const unsigned& w, const unsigned& numFunctionPanel = 0)	//TODO Remove w and h? + remove  default numFunc
				: Panel(0, List::MARGIN, List::MARGIN + numFunctionPanel * (List::NODE_HEIGHT + List::SPACE_BETWEEN)),
				  name(name),
				  background(SDLGLContext::loadTexture("Textures2D/violet.png"), SDLGLContext::loadTexture("Textures2D/violetPale.png"), 0, 0, 0, w, 32),
				  label(1, 0, 0, name, "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}) {
			addComponent("background", &background);
			addComponent("label", &label);
		}
	};

	Rect background;	///< Fond de la liste
	KeyList& keyList;	///< Référence à la KeyList
	std::string held;	///< Nom de la fonction du FunctionPanel tenue par le curseur
	std::pair<int, int> oldPos;	///< Ancienne position du FunctionPanel tenue par le curseur
	std::list<FunctionPanel> functions;	///< Liste de FunctionPanel

public:
	/// \brief Fonction ajoutant une fonction d'une pièce à la liste et à Cockpit
	/// \param std::string Nom de la fonction
	/// \param Piece& Référence à la pièce
	/// \param std::function<void(Piece&)>* Pointeur vers la fonction
	std::function<void(const std::string&, Piece&, std::function<void(Piece&)>*)> functionLoader;

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de pos.x
	/// \param y Valeur initiale de pos.y
	/// \param w Largeur de la liste
	/// \param h Hauteur de la liste
	FunctionList(const int& priority, const int& x, const int& y, const unsigned int& w, const unsigned int& h, KeyList& keyList)
			: Panel(priority, x, y),
			  background(0, 0, 0, w, h, false, Vector3d(.5, .5, .5), false),
			  keyList(keyList) {
		functionLoader = [this, w, &keyList](const std::string& name, Piece& piece, std::function<void(Piece&)>* function) {
			functions.emplace_back(name, w - 20, panels.size());
			addPanel(name, &functions.back());

			functions.back().background.onPress = [this, name](VisualComponent2D& visComp) {
				EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, this);
				oldPos.first = panels[name]->pos.x;
				oldPos.second = panels[name]->pos.y;
				held = name;
			};

			functions.back().background.onUnPress = [this, &keyList, name, function](VisualComponent2D& visComp) {
				EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, this);

				panels[held]->pos.setLocalPos(oldPos.first, oldPos.second);
				keyList.addFunction(name, function);
			};

			functions.back().background.onMouseOver = [&piece](VisualComponent2D& visComp) {
				piece.getModel().setColor(0.7, 0.7, 1.);
			};

			functions.back().background.onMouseAway = [&piece](VisualComponent2D& visComp) {
				piece.getModel().setColor(1., 1., 1.);
			};

			for (std::pair<unsigned, std::vector<std::string>> funcs : PlayerData::commands) {
				bool found = false;

				for (std::vector<std::string>::iterator it = funcs.second.begin(); it != funcs.second.end() && !found; it++)
					if (*it == name) {
						keyList.addFunction(name, function, funcs.first);
						found = true;
					}
			}
		};

		addComponent("background", &background);
	}

	/// \brief Retire le FunctionPanel associée à name
	void removeFunction(const std::string& name) {
		std::list<FunctionPanel>::iterator it = std::find_if(functions.begin(), functions.end(), [&name](const FunctionPanel& functionPanel) {
			return functionPanel.name == name;
		});

		panels.erase(it->name);
		moveElements(it);
		functions.erase(it);
	}

	/// \brief Déplace les FunctionPanel suivant it de 1 vers le haut
	void moveElements(std::list<FunctionPanel>::iterator it) {
		it++;
		for (; it != functions.end(); it++)
			it->pos.y -= List::NODE_HEIGHT + List::SPACE_BETWEEN;
	}

	/// \brief Notification de l'action en cours
	/// \param event Structure contenant l'information sur l'action de l'utilisateur
	void notify(SDL_Event* event) {
		panels[held]->pos.move(event->motion.xrel, event->motion.yrel);
	}
};

#endif //SOURCE_FUNCTIONLIST_H
