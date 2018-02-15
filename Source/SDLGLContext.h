///\brief Initier un contexte OpenGL
///
/// Classe permettant de créer un contexte OpenGL pour permettre à une fenêtre l'environnement 3d

#ifndef SDL2OPENGL_SDLGLCONTEXT_H
#define SDL2OPENGL_SDLGLCONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include "Math.h"
#include "Vector3d.h"
#include "Matrix4d.h"
#include "GameData.h"

class SDLGLContext {
private:
	static SDL_Window* sdlWindow;	///< Contient l'information sur la fenètre
	static SDL_GLContext glContext;	///< Variable de type SDL_GLContext
	static Matrix4d view;	///< Matrice de vue

public:
	SDLGLContext() = delete;

	///\brief Méthode permetant l'initialisation du contexte
	/// \param title Titre de la fenêtre contenant le contexte
	/// \param x Position en x dans l'écran
	/// \param y Position en y dans l'écran
	/// \param w Largeur de la fenêtre contenant le contexte
	/// \param h Hauteur de la fenêtre contenant le contexte
	/// \param flags SDL_WINDOW_FULLSCREEN \ SDL_WINDOW_RESIZABLE
	static void setContext(const char* title, const int& x, const int& y, const int& w, const int& h, const unsigned int& windowFlags) {
		if (sdlWindow)
			SDL_DestroyWindow(sdlWindow);

		sdlWindow = SDL_CreateWindow(title, x, y, w, h, windowFlags | SDL_WINDOW_OPENGL);
		glContext = SDL_GL_CreateContext(sdlWindow);

		glViewport(0, 0, w, h);
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
	}

	~SDLGLContext() {
		SDL_GL_DeleteContext(&glContext);
		SDL_DestroyWindow(sdlWindow);
	}

	///\brief chargement des textures
	///\param filename nom de fichier de la texture
	///\return ID de la texture
	static unsigned int loadTexture(const char* filename, unsigned int textureId = 0) {
		if (!textureId)
			glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		SDL_Surface* surface = IMG_Load(filename);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

		SDL_FreeSurface(surface);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	///\brief chargement des textures
	///\param text texte à écrire
	///\param fontname nom de fichier de la police de caractère
	///\param color couleur du texte
	///\return ID de la texture
	static unsigned int loadLabel(const std::string& text, const char* fontName, SDL_Color color, const double& size, unsigned int textureId = 0) {
		color.b = color.b ^ color.r;
		color.r = color.b ^ color.r;
		color.b = color.b ^ color.r;

		TTF_Font* font = TTF_OpenFont(fontName, size);

		if (!textureId)
			glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		SDL_Surface* surface;

		if (text.empty()) {
			char error = ' ';
			surface = TTF_RenderUTF8_Blended(font, &error, color);
		}else
			surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);

		TTF_CloseFont(font);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		SDL_FreeSurface(surface);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	///\brief clairer le buffer
	static void clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	///\brief changer les buffers
	static void refresh() {
		SDL_GL_SwapWindow(sdlWindow);    //Swaps the buffers
	}

	///\brief Initialisation de la projection 3d
	///\param is3D l'affichage est 3d ou non
	///\param angle Moitié de de l'angle de vue (champ de vision)
	///\param nearPlane Distance entre la caméra et le plan rapproché
	///\param distance Distance entre la caméra et le plan éloigné
	static void setProjection(const bool& is3D, const double& angle, const double& nearPlane, const double& farPlane) {
		int w, h;
		SDL_GetWindowSize(sdlWindow, &w, &h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();    //Sets projectionMatrix to identity

		if (is3D) {
			double right = std::tan(angle / 2) * nearPlane;
			double top = right * ((double)h / (double)w);
			Matrix4d mat;
			mat.setProjection(-right, right, top, -top, nearPlane, farPlane);

			glMultMatrixd(mat.getColumnMajorData());
		}else
			glOrtho(0.0, (double)w, (double)h, 0.0, 1.0, -1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	///\brief Initialisation de la projection 3d
	static void set3dProjection() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();    //Sets projectionMatrix to identity

		Matrix4d mat;
		mat.setProjection(-FRUSTUM_RIGHT, FRUSTUM_RIGHT, FRUSTUM_TOP, -FRUSTUM_TOP, FRUSTUM_NEAR, FRUSTUM_FAR);

		glMultMatrixd(mat.getColumnMajorData());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	///\brief Initialisation de la projection orthographique
	static void set2dProjection() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();    //Sets projectionMatrix to identity

		glOrtho(0.0, (double)WINDOW_WIDTH, (double)WINDOW_HEIGHT, 0.0, 1.0, -1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	///\brief Fonction pour obtenir le vecteur Side de la caméra
	///\return Vecteur Side de la caméra
	static Vector3d getSide() {
		return {view.matrix[0], view.matrix[1], view.matrix[2]};
	}

	///\brief Fonction pour obtenir le vecteur Up de la caméra
	///\return Vecteur Up de la caméra
	static Vector3d getUp() {
		return {view.matrix[4], view.matrix[5], view.matrix[6]};
	}

	///\brief Fonction pour obtenir le vecteur Front de la caméra
	///\return Vecteur Front de la caméra
	static Vector3d getFront() {
		return {-view.matrix[8], -view.matrix[9], -view.matrix[10]};
	}

	///\brief Fonction pour obtenir le ratio (w/h) de la fenêtre
	///\return Valeur du ratio.
	static double getAspectRatio() {
		int w, h;
		SDL_GetWindowSize(sdlWindow, &w, &h);

		return w / h;
	}

	///\brief défini la vue 3d à l'aide de 3 vecteurs
	///\param pos vecteur position de l'utilisateur
	///\param target vecteur cible de l'utilisateur (où celui-ci regarde)
	///\param up vecteur qui pointe vers le haut de l'utilisateur
	static void setView(const Vector3d& pos, const Vector3d& target, const Vector3d& up) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Vector3d front = (target - pos).normalize();

		Vector3d side = (front / up).normalize();

		Vector3d nUp = (side / front).normalize();

		view.setView(side, nUp, front);
		glMultMatrixd(view.getColumnMajorData());

		glTranslated(-pos.x, -pos.y, -pos.z);
	}

	/// \brief Défini le titre de la fenêtre
	/// \param title Texte
	static void setTitle(const std::string& title) {
		SDL_SetWindowTitle(sdlWindow, title.c_str());
	}
};

SDL_Window* SDLGLContext::sdlWindow;
SDL_GLContext SDLGLContext::glContext;
Matrix4d SDLGLContext::view;

#endif //SDL2OPENGL_SDLGLCONTEXT_H
