///\brief Design Pattern Singleton
///
/// Classe permettant d'accéder à un objet unique de type T par fonction statique

#ifndef SOURCE_SINGLETON_H
#define SOURCE_SINGLETON_H

template<typename T>
class Singleton {
private:
	static T* instance;///< Variable contenu dans le Singleton

public:
	///\brief Retourne l'instance du singleton tout en le créant si celui-ci n'existe pas encore
	static T* getInstance() {
		if (!Singleton::instance)
			Singleton::instance = new T();
		return Singleton::instance;
	}
};

template<typename T>
T* Singleton<T>::instance = nullptr;

#endif
