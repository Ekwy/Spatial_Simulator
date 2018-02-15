/// \brief Représentation d'une flamme à l'aide de GL_POINTS
///
/// Collection de points s'élevant et, après avoir franchit length, convergeant vers un point, pour ensuite revenir à la source
///

#ifndef SOURCE_FLAME_H
#define SOURCE_FLAME_H

#include <random>
#include <chrono>
#include "Point.h"
#include "Vector3d.h"
#include "Matrix4d.h"

struct Flame {
	Matrix4d transform; ///< Matrice de transformation
	double strength;	///< Force, vitesse, d'élévation des particules
	double radius;	///< Rayon de la source de flamme
	double length;	///< Longueur du cylindre de la flamme, après lequel les particules commencent à converger
	std::mt19937 RNG;	///< Générateur de nombres aléatoire
	std::uniform_real_distribution<double> spawnDist;	///< Distribution de double générant une position sur la source
	std::uniform_real_distribution<double> deviationDist;	///< Distribution de double générant une force de convergeance
	std::uniform_real_distribution<double> strengthDist;	///< Distribution de double générant une variation de forces
	std::bernoulli_distribution lifeDist;	///< Distribution de bool dictant si la particule doit vieillir de 1
	bool on;	///< Vrai si la flamme doit être mise à jour et dessinée

	/// \brief Structure représentant une particule
	struct Particle {
		Flame& flame;	///< Référence vers la flamme
		Point point;	///< Point contenant la taille, position et couleur de la particule
		Vector3d velocity;	///< Vecteur de vélocité de la particule
		const unsigned lifespan;	///< Expérance de vie de la particule
		unsigned life;	///< Vie de la particule, si life == lifespan, repositionne la particule à la source
		bool old;	///< Vrai si la particule a dépassé flame.length, et doit commencer à vieillir

		Particle(Flame& flame, const Vector3d& color, const double& size, const Vector3d& velocity, const unsigned& lifespan)
				: flame(flame),
				  point(getNewPos(), color, size),
				  velocity(flame.strength * flame.strengthDist(flame.RNG) * flame.transform.getDirection()),
				  lifespan(lifespan),
				  life(0),
				  old(false) {}

		/// \brief Mets à jour la particule
		void update() {
			if (life == lifespan) {
				point.pos = getNewPos();
				old = (bool)(life = 0);
			}

			Vector3d pointAxis(flame.transform.getVectorPointAxis(point.pos));
			double distancePointAxis(pointAxis.getNorm());

			point.pos += velocity;

			if (old) {
				life += flame.lifeDist(flame.RNG) ? 0 : 1;
				point.pos += flame.deviationDist(flame.RNG) * pointAxis;
			}else if (flame.transform.getDirection() * (point.pos - flame.transform.getPos()) > flame.length || distancePointAxis > flame.radius || distancePointAxis < VERY_SMALL_DOUBLE)
				old = true;
		}

		///	\return Le vecteur de la nouvelle position dans la source
		Vector3d getNewPos() const {
			Vector3d direction(flame.transform.getDirection());
			Vector3d perpendicular(direction.getPerpendicular().normalize());
			return flame.transform.getPos() + flame.radius * flame.spawnDist(flame.RNG) * perpendicular + flame.radius * flame.spawnDist(flame.RNG) * (direction / perpendicular).normalize();
		}
	};

	std::vector<Particle> particles;    ///< Vecteur de particules

	/// \param position Vecteur de position initiale de la flamme, représantant le centre de la source
	Flame(const Vector3d& position, const double& strength, const double& radius, const double& length)
			: strength(strength),
			  radius(radius),
			  length(length),
			  RNG(std::chrono::system_clock::now().time_since_epoch().count()),
			  deviationDist(-.1, .2),
			  spawnDist(-1., 1.),
			  strengthDist(.25, 1),
			  lifeDist(0.25),
			  on(false) {
		transform.setIdentity();
		transform.translate(position);
		particles.reserve(50);

		for (unsigned i = 0; i < particles.capacity(); i++) {
			particles.emplace_back(*this, Vector3d(1., (rand() % 100 + 1) / 100.0, 0.), 20., strength * Vector3d::UP, 5);
		}
	}

	/// \brief Mets à jour la flamme
	void update() {
		for (Particle& particle : particles)
			particle.update();
	}

	/// \brief Change la force de la flamme, réinitiant la vélocité des particules
	void setStrength(const double& strength) {
		this->strength = strength;
		for (Particle& particle : particles)
			particle.velocity = strength * strengthDist(RNG) * transform.getDirection();
	}

	/// \brief Applique les transformation d'une matrice 4x4, et fait vieillir les particules
	/// \param Matrice de transformation
	void applyTransform(const Matrix4d& mat) {
		for (Particle& particle : particles) {
			mat.multiply(particle.point.pos);
			mat.multiply(particle.velocity);
			particle.old = true;
		}

		transform = mat * transform;
	}

	/// \brief Dessine la flamme si on = true
	void draw() const {
		if (on)
			for (const Particle& particle : particles)
				particle.point.draw();
	}
};

#endif //SOURCE_FLAME_H
