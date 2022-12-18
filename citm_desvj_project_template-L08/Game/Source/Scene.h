#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	
	//Declaracion de los atributos del Enemigo
	Enemy* enemy;

	enum scenes
	{
		INTRO,
		GAME,
		LOSE,
		WIN
	};

	int scene;

private:
	SDL_Texture* intro;
	SDL_Texture* lose;
	SDL_Texture* win;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	iPoint origin;
	bool originSelected = false;

	int posicion;
	int limiteIzq;
	int limiteDer;

	bool seePath = false;

};

#endif // __SCENE_H__