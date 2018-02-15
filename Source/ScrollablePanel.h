/// \brief Conteneur de VisualComponent2D qui sont défilables
///
/// Permet de contrôler plusieur VisualComponent2D et Panels en même temps.
///

#ifndef SOURCE_SCROLLABLEPANEL_H
#define SOURCE_SCROLLABLEPANEL_H

#include "Panel.h"
#include "ScrollBar.h"

class ScrollablePanel : public Panel {
private:
	ScrollBar scrollbar; ///< Pointeur de scrollbar lié au panel.

public:
	/// \param priority Valeur initiale de priority
	/// \param x Valeur initiale de pos.x
	/// \param y Valeur initiale de pos.y
	ScrollablePanel(const int& priority, const int& x, const int& y)
			: Panel(priority, x, y),
			  scrollbar(SDLGLContext::loadTexture("Textures2D/ScrollBar.png"), SDLGLContext::loadTexture("Textures2D/ScrollBar.png"), 0, true, 100, 444, 20, 79) {

		addComponent("scrollBar", &scrollbar);

		scrollbar.onMouseWheelDown = [this](VisualComponent2D& visComp) {
			for (std::pair<std::string, VisualComponent2D*>& component : components)
				component.second->pos.y++;
		};
		scrollbar.onMouseWheelUp = [this](VisualComponent2D& visComp) {
			for (std::pair<std::string, VisualComponent2D*>& component : components)
				component.second->pos.y--;
		};
	}
};
#endif //SOURCE_SCROLLABLEPANEL_H
