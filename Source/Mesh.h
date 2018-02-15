///\brief Objet dans un environnement 3D
///
///Classe permettant la construction d'un fichier .obj et la modification de son emplacement dans un monde 3D
///

#ifndef SDL2OPENGL_MESH_H
#define SDL2OPENGL_MESH_H

#include <vector>
#include <queue>
#include <fstream>
#include <SDL2/SDL_opengl.h>
#include "Matrix4d.h"
#include "Vector3d.h"
#include "AABB.h"

class Mesh {
private:
	double* vertices; ///< Ensemble des points de l'objet
	double* normals; ///< Ensemble des normals de l'objet
	double* texCoords; ///< Ensemble des coordonnées de texure de l'objet
	unsigned int vertexCount; ///< Nombre de points
	unsigned int textureId; ///< ID de la texture choisie
	AABB aabb;    ///< Boîte AABB du Mesh

	bool applyLight; ///< Si vrai, applique la lumière

	/// \brief Recalcule la grandeur du AABB
	void recalculateAABB() {
		aabb.max.x = aabb.min.x = vertices[0];
		aabb.max.y = aabb.min.y = vertices[1];
		aabb.max.z = aabb.min.z = vertices[2];

		for (unsigned i = 3; i < vertexCount * 3; i += 3) {
			if (aabb.min.x > vertices[i])
				aabb.min.x = vertices[i];
			else if (aabb.max.x < vertices[i])
				aabb.max.x = vertices[i];

			if (aabb.min.y > vertices[i + 1])
				aabb.min.y = vertices[i + 1];
			else if (aabb.max.y < vertices[i + 1])
				aabb.max.y = vertices[i + 1];

			if (aabb.min.z > vertices[i + 2])
				aabb.min.z = vertices[i + 2];
			else if (aabb.max.z < vertices[i + 2])
				aabb.max.z = vertices[i + 2];
		}
	}

public:
	Vector3d color;    ///< Couleur du Mesh

	///\param meshPath Adresse où se trouve le fichier .obj
	///\param textureId ID de la texture du mesh concerné
	///\param hasHitbox Si vrai, crée la hitbox
	Mesh(const char* meshPath, const unsigned int& textureId, const bool& applyLight)
			: textureId(textureId),
			  color(Vector3d::ONE),
			  applyLight(applyLight) {
		loadOBJ(meshPath);

		recalculateAABB();
	}

	~Mesh() {
		delete[] vertices;
		delete[] normals;
		delete[] texCoords;
	}

	/// \brief Aligned-Axis Bounding Box
	const AABB& getAABB() const {
		return aabb;
	}

	/// \return color
	const Vector3d& getColor() const {
		return color;
	}

	/// \brief Défini une nouvelle couleur au Mesh
	/// \param color Nouvelle couleur du Mesh
	void setColor(const Vector3d& color) {
		this->color = color;
	}

	/// \brief Accesseur
	/// \return Référence constante de vertexCount
	const unsigned int& getVertexCount() const {
		return vertexCount;
	}

	/// \brief Accesseur
	/// \return Référence constante de vertices
	const double* getVertices() const {
		return vertices;
	}

	/// \brief Accesseur
	/// \return Référence constante de normals
	const double* getNormals() const {
		return normals;
	}

	///\brief Fonction permettant l'affichage d'un mesh
	virtual void draw() const {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		if (!applyLight)
			glDisable(GL_LIGHTING);

		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		glColor3d(color.x, color.y, color.z);

		if (textureId) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindTexture(GL_TEXTURE_2D, textureId);

			glVertexPointer(3, GL_DOUBLE, 0, vertices);
			glNormalPointer(GL_DOUBLE, 0, normals);
			glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}else {
			glDisable(GL_TEXTURE_2D);

			glVertexPointer(3, GL_DOUBLE, 0, vertices);
			glNormalPointer(GL_DOUBLE, 0, normals);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);

			glEnable(GL_TEXTURE_2D);
		}

		glColor3d(1., 1., 1.);
		glDisable(GL_COLOR_MATERIAL);

		if (!applyLight)
			glEnable(GL_LIGHTING);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	/// \brief Applique une transformation à mesh
	/// \param transform Matrice de transformation
	void transform(const Matrix4d& transform) {
		double vx, vy, vz, nx, ny, nz;
		for (unsigned int i = 0; i < vertexCount * 3; i += 3) {
			vx = transform.matrix[0] * vertices[i] + transform.matrix[1] * vertices[i + 1] + transform.matrix[2] * vertices[i + 2] + transform.matrix[3];
			vy = transform.matrix[4] * vertices[i] + transform.matrix[5] * vertices[i + 1] + transform.matrix[6] * vertices[i + 2] + transform.matrix[7];
			vz = transform.matrix[8] * vertices[i] + transform.matrix[9] * vertices[i + 1] + transform.matrix[10] * vertices[i + 2] + transform.matrix[11];

			nx = transform.matrix[0] * normals[i] + transform.matrix[1] * normals[i + 1] + transform.matrix[2] * normals[i + 2];
			ny = transform.matrix[4] * normals[i] + transform.matrix[5] * normals[i + 1] + transform.matrix[6] * normals[i + 2];
			nz = transform.matrix[8] * normals[i] + transform.matrix[9] * normals[i + 1] + transform.matrix[10] * normals[i + 2];

			vertices[i] = vx;
			vertices[i + 1] = vy;
			vertices[i + 2] = vz;

			normals[i] = nx;
			normals[i + 1] = ny;
			normals[i + 2] = nz;
		}

		recalculateAABB();
	}

	///\brief Permet le déplacement de la mesh
	///\param x Mouvement en X
	///\param y Mouvement en Y
	///\param z Mouvement en Z
	void move(const double& x, const double& y, const double& z) {
		for (int i = 0; i < vertexCount; i++) {
			vertices[i * 3] += x;
			vertices[i * 3 + 1] += y;
			vertices[i * 3 + 2] += z;
		}

		aabb.min.x += x;
		aabb.min.y += y;
		aabb.min.z += z;
		aabb.max.x += x;
		aabb.max.y += y;
		aabb.max.z += z;
	}

	///\brief Permet le déplacement de la mesh
	///\param movement Vecteur de mouvement
	void move(const Vector3d& movement) {
		for (int i = 0; i < vertexCount; i++) {
			vertices[i * 3] += movement.x;
			vertices[i * 3 + 1] += movement.y;
			vertices[i * 3 + 2] += movement.z;
		}

		aabb.min += movement;
		aabb.max += movement;
	}

	/// \brief Permet de faire une rotation autour d'un axe
	/// \param angle Angle de rotation
	/// \param axis Axe de rotation
	/// \param vecs Points à faire tourner autour de l'axe
	/// \param vertexCount Nombre de points à faire tourner
	/// \param target Point origine de l'axe de rotation
	void rotateAroundLine(const double& angle, const Vector3d& axis, const Vector3d& pivot) {
		Math::rotateAroundLine(angle, axis, pivot, vertices, vertexCount);
		Math::rotateAroundLine(angle, axis, pivot, normals, vertexCount);

		recalculateAABB();
	}

	///\brief Fonction permettant un scaling du mesh
	///\param x Scale en x
	///\param y Scale en y
	///\param z Scale en z
	void scale(const double& x, const double& y, const double& z) {
		for (int i = 0; i < vertexCount; i++) {
			vertices[i * 3] *= x;
			vertices[i * 3 + 1] *= y;
			vertices[i * 3 + 2] *= z;
		}

		recalculateAABB();
	}

	///\brief Fonction permettant le loading d'un fichier .obj
	///\param path Chemin pour se rendre au fichier
	void loadOBJ(const std::string& path) {
		std::vector<double> vertexVector;
		std::vector<double> normalVector;
		std::vector<double> texCoordVector;
		std::vector<unsigned int> vIndexVector;
		std::vector<unsigned int> nIndexVector;
		std::vector<unsigned int> tIndexVector;

		std::ifstream fs(path);
		double dBuff;
		unsigned int uiBuff;
		std::string str;

		while (!fs.eof()) {
			str.clear();
			fs >> str;

			switch (str[0]) {
				case 'v':
					if (str.length() > 1) {
						switch (str[1]) {
							case 't':
								fs >> dBuff;
								texCoordVector.push_back(dBuff);
								fs >> dBuff;
								texCoordVector.push_back(1 - dBuff);
								break;
							case 'n':
								for (unsigned char i = 0; i < 3; i++) {
									fs >> dBuff;
									normalVector.push_back(dBuff);
								}
								break;
						}
					}else
						for (unsigned char i = 0; i < 3; i++) {
							fs >> dBuff;
							vertexVector.push_back(dBuff);
						}
					break;
				case 'f':
					for (unsigned int i = 0; i < 3; i++) {
						fs >> uiBuff;
						vIndexVector.push_back(uiBuff - 1);
						fs.ignore();

						fs >> uiBuff;
						tIndexVector.push_back(uiBuff - 1);
						fs.ignore();

						fs >> uiBuff;
						nIndexVector.push_back(uiBuff - 1);
					}
					break;
				default:
					fs.ignore(30, 10);
			}
		}

		fs.close();

		vertexCount = (unsigned int)vIndexVector.size();

		vertices = new double[3 * vertexCount];
		normals = new double[3 * vertexCount];
		texCoords = new double[2 * vertexCount];

		for (unsigned int i = 0; i < vertexCount; i++) {
			vertices[i * 3] = vertexVector[vIndexVector[i] * 3];
			vertices[i * 3 + 1] = vertexVector[vIndexVector[i] * 3 + 1];
			vertices[i * 3 + 2] = vertexVector[vIndexVector[i] * 3 + 2];

			normals[i * 3] = normalVector[nIndexVector[i] * 3];
			normals[i * 3 + 1] = normalVector[nIndexVector[i] * 3 + 1];
			normals[i * 3 + 2] = normalVector[nIndexVector[i] * 3 + 2];

			texCoords[i * 2] = texCoordVector[tIndexVector[i] * 2];
			texCoords[i * 2 + 1] = texCoordVector[tIndexVector[i] * 2 + 1];
		}
	}
};

#endif //SDL2OPENGL_MESH_H
