/// \brief Gestion des événements.
///
/// Classe permettant d'ajouter des évènements observables et d'enregistrer des observateurs à certains évènements.
///

#ifndef SOURCE_EVENTDISPATCHER_H
#define SOURCE_EVENTDISPATCHER_H

#include <map>
#include <SDL2/SDL.h>
#include "Observable.h"

class EventDispatcher {
private:
	static std::map<unsigned int, Observable<SDL_Event*>> events;	///< table de hachage contenant des événements SDL associés à des observables
	static const Uint8* keyState;	///< Tableaux de bool vrai si la touche respective est pressée

public:
	static SDL_Event sdlEvent;	///< événement de la librairie SDL

	/// \brief gestion des évènements SDL et notification des observateurs
	/// \param running programme en court ou terminé
	static void pollEvents(bool& running) {
		keyState = SDL_GetKeyboardState(NULL);

		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
			else {
				if (events.find(sdlEvent.type) == events.end())
					events.insert(std::make_pair(sdlEvent.type, Observable<SDL_Event*>()));
				events[sdlEvent.type].notifyObservers(&sdlEvent);
			}
		}
	}

	/// \param key Scancode de la touche à tester
	/// \return Vrai si la touche en enfoncée
	static bool getState(const unsigned int& key) {
		return keyState[key];
	}

	/// \brief enregistrement d'un observateur à un évènement
	/// \param eventType type d'évènement SDL
	/// \param observer observateur qui regarde des évènements
	static void registerEventObserver(unsigned int eventType, Observer<SDL_Event*>* observer) {
		if (events.find(eventType) == events.end())
			events.insert(std::make_pair(eventType, Observable<SDL_Event*>()));
		events[eventType].registerObserver(observer);
	}

	/// \brief désenregistrement d'un observateur à un évènement
	/// \param eventType type d'évènement SDL
	/// \param observer observateur qui regarde des évènements
	static void unregisterEventObserver(unsigned int eventType, Observer<SDL_Event*>* observer) {
		if (events.find(eventType) != events.end())
			events[eventType].unregisterObserver(observer);
	}
};

SDL_Event EventDispatcher::sdlEvent;
std::map<unsigned int, Observable<SDL_Event*>> EventDispatcher::events;
const Uint8* EventDispatcher::keyState = nullptr;

#endif //SOURCE_EVENTDISPATCHER_H
