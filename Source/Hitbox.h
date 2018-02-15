///\brief Boite de collision d'un objet dans un environnement 3D
///
/// Classe permettant la construction d'une boite de collision à partir d'un Mesh
///

#ifndef SOURCE_HITBOX_H
#define SOURCE_HITBOX_H

#include "Matrix4d.h"

class Hitbox {
private:
	double* vertices;	///< Ensemble des points de la boite de collisions
	double* normals;	///< Ensemble des normals de la boite de collisions
	unsigned int* indices;	///< Ensemble des indices de la boite de collisions

public:
	///\param lowX Point en X le plus bas
	///\param highX Point en X le plus haut
	///\param lowY Point en Y le plus bas
	///\param highY Point en Y le plus haut
	///\param lowZ Point en Z le plus bas
	///\param highZ Point en Z le plus haut
	Hitbox(const double& lowX, const double& highX, const double& lowY, const double& highY, const double& lowZ, const double& highZ) {
		vertices = new double[72];

		vertices[0] = lowX;		vertices[1] = highY;	vertices[2] = highZ;	// 0  FRONT
		vertices[3] = highX;	vertices[4] = highY;	vertices[5] = highZ;	// 1
		vertices[6] = lowX;		vertices[7] = lowY;		vertices[8] = highZ;	// 2
		vertices[9] = highX;	vertices[10] = lowY;	vertices[11] = highZ;	// 3

		vertices[12] = lowX;	vertices[13] = highY;	vertices[14] = lowZ;	// 4  BACK
		vertices[15] = highX;	vertices[16] = highY;	vertices[17] = lowZ;	// 5
		vertices[18] = lowX;	vertices[19] = lowY;	vertices[20] = lowZ;	// 6
		vertices[21] = highX;	vertices[22] = lowY;	vertices[23] = lowZ;	// 7

		vertices[24] = lowX;	vertices[25] = highY;	vertices[26] = lowZ;	// 8  TOP
		vertices[27] = highX;	vertices[28] = highY;	vertices[29] = lowZ;	// 9
		vertices[30] = lowX;	vertices[31] = highY;	vertices[32] = highZ;	// 10
		vertices[33] = highX;	vertices[34] = highY;	vertices[35] = highZ;	// 11

		vertices[36] = lowX;	vertices[37] = lowY;	vertices[38] = lowZ;	// 12  BOT
		vertices[39] = highX;	vertices[40] = lowY;	vertices[41] = lowZ;	// 13
		vertices[42] = lowX;	vertices[43] = lowY;	vertices[44] = highZ;	// 14
		vertices[45] = highX;	vertices[46] = lowY;	vertices[47] = highZ;	// 15

		vertices[48] = lowX;	vertices[49] = highY;	vertices[50] = lowZ;	// 16  LEFT
		vertices[51] = lowX;	vertices[52] = highY;	vertices[53] = highZ;	// 17
		vertices[54] = lowX;	vertices[55] = lowY;	vertices[56] = lowZ;	// 18
		vertices[57] = lowX;	vertices[58] = lowY;	vertices[59] = highZ;	// 19

		vertices[60] = highX;	vertices[61] = highY;	vertices[62] = lowZ;	// 20 RIGHT
		vertices[63] = highX;	vertices[64] = highY;	vertices[65] = highZ;	// 21
		vertices[66] = highX;	vertices[67] = lowY;	vertices[68] = lowZ;	// 22
		vertices[69] = highX;	vertices[70] = lowY;	vertices[71] = highZ;	// 23


		normals = new double[72];
		//FRONT
		normals[0] = 0.0;	normals[1] = 0.0;	normals[2] = 1.0;
		normals[3] = 0.0;	normals[4] = 0.0;	normals[5] = 1.0;
		normals[6] = 0.0;	normals[7] = 0.0;	normals[8] = 1.0;
		normals[9] = 0.0;	normals[10] = 0.0;	normals[11] = 1.0;

		//BACK
		normals[12] = 0.0;	normals[13] = 0.0;	normals[14] = -1.0;
		normals[15] = 0.0;	normals[16] = 0.0;	normals[17] = -1.0;
		normals[18] = 0.0;	normals[19] = 0.0;	normals[20] = -1.0;
		normals[21] = 0.0;	normals[22] = 0.0;	normals[23] = -1.0;


		//TOP
		normals[24] = 0.0;	normals[25] = 1.0;	normals[26] = 0.0;
		normals[27] = 0.0;	normals[28] = 1.0;	normals[29] = 0.0;
		normals[30] = 0.0;	normals[31] = 1.0;	normals[32] = 0.0;
		normals[33] = 0.0;	normals[34] = 1.0;	normals[35] = 0.0;

		//BOT
		normals[36] = 0.0;	normals[37] = -1.0;	normals[38] = 0.0;
		normals[39] = 0.0;	normals[40] = -1.0;	normals[41] = 0.0;
		normals[42] = 0.0;	normals[43] = -1.0;	normals[44] = 0.0;
		normals[45] = 0.0;	normals[46] = -1.0;	normals[47] = 0.0;


		//LEFT
		normals[48] = -1.0;	normals[49] = 0.0;	normals[50] = 0.0;
		normals[51] = -1.0;	normals[52] = 0.0;	normals[53] = 0.0;
		normals[54] = -1.0;	normals[55] = 0.0;	normals[56] = 0.0;
		normals[57] = -1.0;	normals[58] = 0.0;	normals[59] = 0.0;

		//RIGHT
		normals[60] = 1.0;	normals[61] = 0.0;	normals[62] = 0.0;
		normals[63] = 1.0;	normals[64] = 0.0;	normals[65] = 0.0;
		normals[66] = 1.0;	normals[67] = 0.0;	normals[68] = 0.0;
		normals[69] = 1.0;	normals[70] = 0.0;	normals[71] = 0.0;


		indices = new unsigned int[36];
		//FRONT
		indices[0] = 0;		indices[1] = 1;		indices[2] = 2;
		indices[3] = 1;		indices[4] = 2;		indices[5] = 3;

		//BACK
		indices[6] = 4;		indices[7] = 5;		indices[8] = 6;
		indices[9] = 5;		indices[10] = 6;	indices[11] = 7;

		//TOP
		indices[12] = 8;	indices[13] = 9;	indices[14] = 10;
		indices[15] = 9;	indices[16] = 10;	indices[17] = 11;

		//BOT
		indices[18] = 12;	indices[19] = 13;	indices[20] = 14;
		indices[21] = 13;	indices[22] = 14;	indices[23] = 15;

		//LEFT
		indices[24] = 16;	indices[25] = 17;	indices[26] = 18;
		indices[27] = 17;	indices[28] = 18;	indices[29] = 19;

		//RIGHT
		indices[30] = 20;	indices[31] = 21;	indices[32] = 22;
		indices[33] = 21;	indices[34] = 22;	indices[35] = 23;

	}

	~Hitbox() {
		delete[] vertices;
		delete[] normals;
		delete[] indices;
	}

	/// \return Référence constante à vertices
	const double* getVertices() const {
		return vertices;
	}

	/// \return Référence constante à normals
	const double* getNormals() const {
		return normals;
	}

	/// \return Référence constante à indices
	const unsigned int* getIndices() const {
		return indices;
	}

	/// \brief Applique une transformation à la hitbox
	/// \param transform Matrice de transformation
	void transform(const Matrix4d& transform) {
		double vx, vy, vz, nx, ny, nz;
		for (unsigned int i = 0; i < 24 * 3; i += 3) {
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
	}

	///\brief Fonction permettant le déplacement d'une boite de collisions
	///\param x Mouvement en X
	///\param y Mouvement en Y
	///\param z Mouvement en Z
	void move(const double& x, const double& y, const double& z) {
		for (int i = 0; i < 24; i++) {
			vertices[i * 3] += x;
			vertices[i * 3 + 1] += y;
			vertices[i * 3 + 2] += z;
		}
	}

	///\brief Permet le déplacement d'une boite de collisions
	///\param movement Vecteur de mouvement
	void move(const Vector3d& movement) {
		for (int i = 0; i < 24; i++) {
			vertices[i * 3] += movement.x;
			vertices[i * 3 + 1] += movement.y;
			vertices[i * 3 + 2] += movement.z;
		}
	}

	///\brief Fonction permettant un scaling de la hitbox
	///\param x Scale en x
	///\param y Scale en y
	///\param z Scale en z
	void scale(const double& x, const double& y, const double& z) {
		for (int i = 0; i < 24; i++) {
			vertices[i * 3] *= x;
			vertices[i * 3 + 1] *= y;
			vertices[i * 3 + 2] *= z;
		}
	}

	///\brief Fonction permettant la rotation d'une boite de collisions selon une matrice de rotation
	///\param rotationMat Matrice 4D de rotation
	void rotate(const Matrix4d& rotationMat) {
		double vx, vy, vz, nx, ny, nz;
		for (int i = 0; i < 24; i++) {
			vx = vertices[i * 3];
			vy = vertices[i * 3 + 1];
			vz = vertices[i * 3 + 2];
			vertices[i * 3] = rotationMat.matrix[0] * vx + rotationMat.matrix[1] * vy + rotationMat.matrix[2] * vz;
			vertices[i * 3 + 1] = rotationMat.matrix[4] * vx + rotationMat.matrix[5] * vy + rotationMat.matrix[6] * vz;
			vertices[i * 3 + 2] = rotationMat.matrix[8] * vx + rotationMat.matrix[9] * vy + rotationMat.matrix[10] * vz;

			nx = normals[i * 3];
			ny = normals[i * 3 + 1];
			nz = normals[i * 3 + 2];
			normals[i * 3] = rotationMat.matrix[0] * nx + rotationMat.matrix[1] * ny + rotationMat.matrix[2] * nz;
			normals[i * 3 + 1] = rotationMat.matrix[4] * nx + rotationMat.matrix[5] * ny + rotationMat.matrix[6] * nz;
			normals[i * 3 + 2] = rotationMat.matrix[8] * nx + rotationMat.matrix[9] * ny + rotationMat.matrix[10] * nz;
		}
	}

	/// \brief Permet de faire une rotation autour d'un axe
	/// \param angle Angle de rotation
	/// \param axis Axe de rotation
	/// \param pivot Point origine de l'axe de rotation
	void rotateAroundLine(const double& angle, const Vector3d& axis, const Vector3d& pivot) {
		Math::rotateAroundLine(angle, axis, pivot, vertices, 24);
		Math::rotateAroundLine(angle, axis, pivot, normals, 24);
	}

	/// \brief Dessine la HitBox
	void draw() {
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glColor3d(0., 1., 0.);

		glVertexPointer(3, GL_DOUBLE, 0, vertices);
		glNormalPointer(GL_DOUBLE, 0, normals);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

		glColor3d(1., 1., 1.);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};

#endif
