#ifndef SOURCE_MAINMENUSCENE_H
#define SOURCE_MAINMENUSCENE_H

#include <list>
#include "Scene.h"
#include "ModelGroup.h"
#include "Atmosphere.h"
#include "Button.h"
#include "Label.h"
#include "Model.h"
#include "Vector3d.h"
#include "Matrix4d.h"
#include "Easer.h"

class MainMenuScene : public Scene {
private:
	ModelGroup objects; ///< Objets à affichage proche
	ModelGroup farObjects; ///< Objets à affichage loin
	Atmosphere atmosphere; ///< Atmosphère de la scène
	Model planet; ///< Modèle de la planète Terre
	Label label; ///< Titre du projet
	unsigned numSaves; ///< Nombre de sauvegarde
	double segmentAngle; ///< Rotation en rad à effectuer à chaque left ou right
	Button left; ///< Button de flêche gauche
	Button right; ///< Button de flêche droite

	struct SavePanel : public Panel {
		const std::string name;
		Button background;
		Label label;
		Easer<1> easer;

		SavePanel(const std::string& name, const unsigned& radius, const double& angle, std::function<void(VisualComponent2D&)> switchScene)
				: Panel(0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - radius),
				  name(name),
				  background(SDLGLContext::loadTexture("Textures2D/blue.png"), SDLGLContext::loadTexture("Textures2D/bluePale.png"), 0, 0, 0, 125, 40),
				  label(0, 0, 0, name, "Fonts/OpenSans/OpenSans-Regular.ttf", 18, {255, 255, 255, 255}) {
			addComponent("background", &background);
			addComponent("label", &label);

			unsigned bgXRadius(background.w / 2);
			unsigned bgYRadius(background.h / 2);
			unsigned lblXRadius(label.w / 2);
			unsigned lblYRadius(label.h / 2);

			background.pos.x -= bgXRadius;
			background.pos.y -= bgYRadius;

			label.pos.x -= lblXRadius;
			label.pos.y -= lblYRadius;

			background.pivot.first = bgXRadius;
			background.pivot.second = bgYRadius + radius;
			label.pivot.first = lblXRadius;
			label.pivot.second = lblYRadius + radius;


			background.onClick = switchScene;

			background.angle = label.angle = angle/* + Math::degToRad(WINDOW_WIDTH) / 1000*/;

			easer.elementList.push_back({&background.angle});
			easer.elementList.push_back({&label.angle});
		}
	};

	std::list<SavePanel> saves;

protected:
	/// \brief Exécute les instructions pendant la mise à jour à chaque tick
	void periUpdate() {
		Scene::periUpdate();

		for (auto& save : saves)
			save.easer.update();

		planet.rotateAroundLine(planetAxis, Vector3d::ZERO, (double)0.1_deg);
	}

	/// \brief Dessine le 3d de la scène
	void draw3d() {
		SDLGLContext::set3dProjection();
		Singleton<Camera>::getInstance()->setView();

		//glLightiv(GL_LIGHT0, GL_POSITION, new int[4]{0, -2, -1, 0});
		glLightfv(GL_LIGHT0, GL_POSITION, new float[4]{(float)Singleton<Camera>::getInstance()->pos.x, (float)Singleton<Camera>::getInstance()->pos.y, (float)Singleton<Camera>::getInstance()->pos.z, 0.});

		for (std::pair<std::string, ModelGroup&> modelGroup : modelGroups)
			modelGroup.second.draw();
	}

public:
	const Vector3d planetAxis; ///< Orientation de la planète

	MainMenuScene() : Scene(),
					  atmosphere("Textures3D/BuildScene/skybox_blue.obj", SDLGLContext::loadTexture("Textures3D/BuildScene/Skins/spacebox.png"), 60.0, 0., 500., 2'000.),
					  label(0, 0, 50, "P.I.", "Fonts/OpenSans/OpenSans-ExtraBold.ttf", 72, {255, 255, 255, 255}),
					  planet("Textures3D/sphere.obj", SDLGLContext::loadTexture("Textures3d/EarthImageV2.png"), true),
					  planetAxis(0.1, 1., 0.),
					  numSaves(6),
					  segmentAngle(2 * PI / numSaves),
					  left(SDLGLContext::loadTexture("Textures2D/leftArrow.png"), SDLGLContext::loadTexture("Textures2D/leftArrowPressed.png"), 0, WINDOW_WIDTH / 2 - 200, 200),
					  right(SDLGLContext::loadTexture("Textures2D/rightArrow.png"), SDLGLContext::loadTexture("Textures2D/rightArrowPressed.png"), 0, WINDOW_WIDTH / 2 + 175, 200) {
		addModelGroup("objects", objects);
		addFarModelGroup("farObjects", farObjects);

		farObjects.addModel("atmosphere", atmosphere);
		objects.addModel("planet", planet);
		planet.scale(3.);
		planet.applyTransform(Matrix4d().set(Math::rotationBetween(Vector3d::UP, planetAxis)));

		mainPanel.addComponent("label", &label);
		label.pos.x = WINDOW_WIDTH / 2 - label.w / 2;

		std::function<void(VisualComponent2D&)> switchScene = [this](VisualComponent2D& visComp) {
			this->nextScene = "BuildScene";
		};

		for (unsigned i = 0; i < numSaves; i++) {
			saves.emplace_back("Save " + std::to_string(i), 185, i * segmentAngle, switchScene);
			mainPanel.addPanel(saves.back().name, &saves.back());
		}

		addComponent("left", &left);
		addComponent("right", &right);

		left.onClick = [this](VisualComponent2D& visComp) {
			rotateSaves(false);
		};

		right.onClick = [this](VisualComponent2D& visComp) {
			rotateSaves(true);
		};

		EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
		EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, this);

		glEnable(GL_LIGHT1);
		float* lightData = new float[4]{0.8, 0.8, 0.8, 0.};
		glLightfv(GL_LIGHT1, GL_AMBIENT, lightData);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightData);
		delete[] lightData;
	}

	~MainMenuScene() {
		glDisable(GL_LIGHT1);

		EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);
		EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, this);
	}

	/// \brief Tourne les saves
	/// \param right Si vrai, tourne vers la droite, sinon vers la gauche
	void rotateSaves(const bool& right) {
		if (!saves.back().easer.isMoving()) {
			if (right)
				for (auto& save : saves)
					save.easer.setTarget(40, {save.background.angle - segmentAngle});
			else
				for (auto& save : saves)
					save.easer.setTarget(40, {save.background.angle + segmentAngle});
		}
	}

	/// \brief Mise à jour de la scène
	void notify(SDL_Event* event) {
		if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.sym == SDLK_LEFT || event->key.keysym.sym == SDLK_a)
				rotateSaves(false);
			else if (event->key.keysym.sym == SDLK_RIGHT || event->key.keysym.sym == SDLK_d)
				rotateSaves(true);
		}
		/*else {
			for (auto& save : saves) {
				double angle(Math::degToRad(event->motion.xrel) / 500);
				save.background.angle -= angle;
				save.label.angle -= angle;
			}
		}*/
	}
};

#endif //SOURCE_MAINMENUSCENE_H
