#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "Model.h"

/// \brief Représentation de l'atmosphère

class Atmosphere : public Model {
private:
	double levelRatio;	///< Ratio du niveau
	double* atmosphereVertex;	///< Sommets de la boîte atmosphérique.
	const double minHeight;	///< Distance maximale à laquelle levelRatio = 1.
	const double maxHeight;	///< Distance à laquelle levelRatio = 0.

public:
	Atmosphere(const char* fileName, const unsigned int& textureID, double r, const double& levelRatio, const double& minHeight, const double& maxHeight)
			: Model(fileName, textureID, true),
			  levelRatio(levelRatio),
			  minHeight(minHeight),
			  maxHeight(maxHeight) {
		Model::scale(4 * r);
		atmosphereVertex = new double[72]{-r, r, r, r, r, r, r, -r, r, -r, -r, r, -r, r, -r, r, r, -r, r, -r, -r, -r, -r, -r, -r, r, r, -r, r, -r, r, r, -r, r, r, r, -r, -r, r, -r, -r, -r, r, -r, -r, r, -r, r, -r, r, r, -r, r, -r, -r, -r, -r, -r, -r, r, r, r, r, r, r, -r, r, -r, -r, r, -r, r};
	}

	/// \brief Dessine l'atmosphère
	void draw() const {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		Model::draw();

		glDisable(GL_TEXTURE_2D);
		glColor4d(0.40, 0.698, 1., levelRatio);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, atmosphereVertex);
		glDrawArrays(GL_QUADS, 0, 24);
		glDisableClientState(GL_VERTEX_ARRAY);
		glColor4d(1., 1., 1., 1.);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	/// \brief Déplace l'atmosphère de movement
	/// \param movement Déplacement
	void move(const Vector3d& movement) {
		Model::move(movement);

		for (int i = 0; i < 72; i += 3) {
			atmosphereVertex[i] += movement.x;
			atmosphereVertex[i + 1] += movement.y;
			atmosphereVertex[i + 2] += movement.z;
		}
	}

	/// \brief Effectue un grandissement de factor sur l'atmosphère
	/// \param Facteur de grandissement
	void scale(const double& factor) {
		Model::scale(factor);

		for (unsigned i = 0; i < 72; i++)
			atmosphereVertex[i] *= factor;
	}

	/// \brief Détermine le ratio du niveau
	/// \param distanceFromPlanet Distance de la planète la plus proche
	void setLevel(const double& distanceFromPlanet) {
		if (distanceFromPlanet > minHeight)
			if (distanceFromPlanet > maxHeight)
				levelRatio = 0.;
			else
				levelRatio = 1. - (distanceFromPlanet - minHeight) / maxHeight;
		else
			levelRatio = 1.;
	}

	/// \return levelRatio
	const double& getLevel() {
		return levelRatio;
	}
};

#endif
