/// \brief ( ͡° ͜ʖ ͡°)
///
/// Classe qui te regarde.
///

#ifndef SOURCE_OBSERVER_H
#define SOURCE_OBSERVER_H

template <typename T>
class Observer {
public:
  	/// \brief Méthode virtuelle pure permettant d'alerter un objet.
  	/// \param arg Objet en question.
	virtual void notify(T arg) = 0;
};

#endif //SOURCE_OBSERVER_H
