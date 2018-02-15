///\brief élément observalbe
///
/// Classe permettant de rendre un élément observable par des observateurs.
///

#ifndef SOURCE_OBSERVABLE_H
#define SOURCE_OBSERVABLE_H

#include <list>
#include <algorithm>
#include "Observer.h"

template <typename T>
class Observable {
private:
	std::list<Observer<T>*> observers; ///< Liste de tous les objets se faisant observer

public:
	/// \brief Enregistre un objet qui doit se faire observer
	/// \param observer Pointeur de l'objet qui se fait observer
	void registerObserver(Observer<T>* observer) {
		if (observer && (std::find(observers.begin(), observers.end(), observer) == observers.end()))
			observers.push_back(observer);
	}

	/// \brief Désenregistre un objet observé.
	/// \param observer Pointeur de l'objet qui se fait observer
	void unregisterObserver(Observer<T>* observer) {
		observers.remove(observer);
	}

	/// \brief Notifie les objets observés
	/// \param arg Ce qu'on a besoin de notifier.
	void notifyObservers(T arg) {
		for (Observer<T>* it: observers)
			it->notify(arg);
	}
};

#endif //SOURCE_OBSERVABLE_H
